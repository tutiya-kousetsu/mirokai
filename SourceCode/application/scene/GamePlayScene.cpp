#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "TouchableObject.h"
#include "Easing.h"
#include <fstream>
#include <cassert>

void GamePlayScene::Initialize(DirectXCommon* dxCommon)
{

	Audio* audio = Audio::GetInstance();
	//スプライト読み込み
	//ポストエフェクトの初期化
	postEffect.reset(new PostEffect());
	postEffect->CreateGraphicsPipelineState(L"Resources/shaders/PostEffectPS.hlsl");
	//シェーダーの挿入
	postEffect->Initialize();

	//カメラの初期化
	camera.reset(new FollowingCamera());
	//カメラを3Dオブジェットにセット
	Object3d::SetCamera(camera.get());
	debugCam.reset(new DebugCamera(WinApp::window_width, WinApp::window_height));

	//デバイスをセット
	Fbx_Object3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	Fbx_Object3d::SetCamera(camera.get());
	//グラフィックスパイプライン生成
	Fbx_Object3d::CreateGraphicsPipeline();

	//乱数の初期化
	srand((unsigned)time(NULL));

	//当たり判定のインスタンス
	collisionMan = CollisionManager::GetInstance();
	//自機のオブジェクトセット+初期化
	player.reset(Player::Create(Model::CreateFromOBJ("octopus")));
	player->SetPosition({ 0.f, 30.f, 0.f });
	player->SetScale({ 0.f,0.f,0.f });
	//player->SetCollider(new SphereCollider());
	//パーティクのインスタンス
	particleMan = ParticleManager::GetInstance();
	particleMan->Initialize();
	particleMan->SetCamera(camera.get());

	//データ読み込み
	skyObj = Object3d::Create();
	skyObj->SetModel(Model::CreateFromOBJ("skydome"));


	//マップチップ読み込み用
	LoadFloorPopData();
}

void GamePlayScene::Finalize()
{
	//解放
	camera.reset();
	skyObj.reset();
	postEffect.reset();
	player.reset();
	CollisionManager::GetInstance()->RemoveCollider(collider);
	delete collider;
	delete input;
	floors.remove_if([](std::unique_ptr<Floor>& obstacle) {
		return obstacle->GetAlive();
		});
}

void GamePlayScene::Update()
{
		
	UpdataFloorPopCommand();
	for (auto& floor : floors) {
		floor->Update();
	}
	Input* input = Input::GetInstance();

	//カメラにプレイヤーを固定させる
	camera->SetFollowingTarget(player.get());

	//プレイヤーの更新
	player->Update();


	playerPos = player->GetPosition();
	//自機のHPが0になったら小さくする
	if (!player->GetAlive()) {
		player->ScaleSmall();
	}
	//自機がステージから落ちたら小さくする
	if (playerPos.y <= -10.0f) {
		player->ScaleSmall();
	}
	//プレイヤーのHPが0になったらポストエフェクト
	if ( playerPos.y <= -10.0f) {
		//中心に向かってポストエフェクトで暗くする
		endEfRadius = postEffect->GetRadius();
		endEfRadius -= 10.5f;
		if (endEfRadius <= 0.f) {
			endEfRadius = 0;
			endFlag = true;
		}
		postEffect->SetRadius(endEfRadius);
	}
	if (endFlag) {
		//シーン切り替え
		BaseScene* scene = new Tutorial();
		this->sceneManager->SetNextScene(scene);
	}


	player->SetPosition(playerPos);
	//カメラの更新
	camera->Update();
	debugCam->Update();

	skyObj->Update();

	//当たり判定
	CheckAllCollision();
	collisionMan->CheckAllCollisions();

	particleMan->Update();

}


void GamePlayScene::Draw(DirectXCommon* dxCommon)
{
	//描画前処理
	//dxCommon->PreDraw();
	//スプライト描画
#pragma region 背景スプライト描画
	postEffect->PreDrawScene(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion

	//3Dオブジェクト描画前処理
	Object3d::PreDraw();
	//奥にあるほど先に書く
	skyObj->Draw();
	for (auto& floor : floors) {
		floor->Draw();
	}

	player->Draw();
		particleMan->Draw();
	Object3d::PostDraw();

	postEffect->PostDrawScene(dxCommon->GetCmdList());

	//描画前処理
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCmdList());

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	// スプライト描画後処理

	Sprite::PostDraw();
	//描画後処理
	dxCommon->PostDraw();

#pragma region 前景スプライト描画
}

void GamePlayScene::LoadFloorPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/csv/FloorPop.csv");
	assert(file.is_open());

	//ファイル内容を文字列ストリームにコピー
	floorPopCom << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdataFloorPopCommand()
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

void GamePlayScene::CheckAllCollision()
{
	audio = Audio::GetInstance();
}

