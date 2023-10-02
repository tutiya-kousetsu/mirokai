#include "Tutorial.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "GamePlayScene.h"
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MeshCollider.h"
#include "ParticleManager.h"
#include "Easing.h"
#include <fstream>

void Tutorial::Initialize(DirectXCommon* dxCommon)
{
	Audio* audio = Audio::GetInstance();


	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラの初期化
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//床のオブジェクト生成
	//floor.reset(TouchableObject::Create(Model::CreateFromOBJ("FloorBox")));
	//floor->SetPosition({ 0,-18.5f,0 });

	//データ読み込み
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));


	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("Box")));
	player->SetPosition({ 0, 10.f, 0 });

	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//シェーダーの挿入
	postEffect->Initialize();

	//マップチップ読み込み用
	LoadFloorPopData();
}

void Tutorial::Finalize()
{
}

void Tutorial::Update()
{


	audio = Audio::GetInstance();
	if (!startFlag) {
		//中心から明るくする
		startEfRadius += 10.5f;
		if (startEfRadius >= 1000) {
			startFlag = true;
			startEfRadius -= 10.5f;
		}
	}
	Input* input = Input::GetInstance();

	//カメラを3Dオブジェットにセット
	nowCamera = camera.get();
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(nowCamera);

	camera->SetFollowingTarget(player.get());

	//if (rotateFlag) {
		//rotateTime--;
		////追従カメラから普通のカメラに変更
		//nowCamera = debugCam.get();
		//Object3d::SetCamera(nowCamera);
		//nowCamera->SetEye({ player->GetPosition().x, 2.0f, 0 });
		//nowCamera->SetTarget(player->GetPosition());
		////回転させる
		//playerRot = player->GetRotation();
		//playerRot.y += 3.0f;
		//player->SetRotation(playerRot);
		//if (rotateTime <= 0) {
		//	//タイムが0になったら自機を上にあげる
		//	playerPos = player->GetPosition();
		//	if (easFrame < 1.0f) {
		//		easFrame += 0.01f;
		//		//自機の大きさを小さくさせる関数
		//		player->ScaleSmall();
		//	}

		//	playerPos.y = Ease(In, Cubic, easFrame, playerPos.y, 40.0f);
		//	playerScale = player->GetScale();
		//	//プレイヤーのスケールが0になったらシーン移動
		//	if (playerScale.x <= 0.f && playerScale.y <= 0.f && playerScale.z <= 0.f) {
		//		BaseScene* scene = new GamePlayScene();
		//		this->sceneManager->SetNextScene(scene);
		//	}
		//	player->SetPosition(playerPos);
		//}
		//player->StopUpdate();
	//}


	//プレイヤーがステージから落ちたらステージの真ん中に戻す
	if (player->GetPosition().y <= -20) {
		player->SetPosition({ 0,10,0 });
		player->SetRotation({ 0,0,0 });
	}

	//各オブジェクトの更新
	postEffect->SetRadius(startEfRadius);
	skyObj->Update();
	player->Update();
	UpdataFloorPopCommand();
	for (auto& floor : floors) {
		floor->Update();
	}
	camera->Update();
	debugCam->Update();

	//当たり判定
	CheckAllCollision();
	particleMan->Update();
}

void Tutorial::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	//dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	postEffect->PreDrawScene(dxCommon->GetCmdList());

	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());
	//背景スプライト描画

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer(dxCommon->GetCmdList());
#pragma endregion

#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	skyObj->Draw();
	player->Draw();
	for (auto& floor : floors) {
		floor->Draw();
	}

	if (!rotateFlag) {
		particleMan->Draw();
	}
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//描画前処理
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

	//描画後処理
	dxCommon->PostDraw();
}

void Tutorial::LoadFloorPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/tutorial.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	floorPopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void Tutorial::UpdataFloorPopCommand()
{
	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(floorPopCom, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			//コンストラクタ呼ぶ
			std::unique_ptr<Floor> newFloor = std::make_unique<Floor>();
			newFloor->Initialize(XMFLOAT3(x, y, z));
			//障害物を登録する
			floors.push_back(std::move(newFloor));
		}
	}
}


void Tutorial::CheckAllCollision()
{
#pragma endregion

}
