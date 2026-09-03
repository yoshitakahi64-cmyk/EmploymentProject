#pragma once

#include <DxLib.h>
#include "../Library/json/json.hpp"
#include "../Quatrenion/Quatrenion.h"
#include <string>

// UnityのGameObjectの情報を格納する構造体
struct GameObject_Unity
{
	int id = -1;
    VECTOR position_3D = {};
    VECTOR scale = {};
	Quaternion rotation = {};
	std::string name = "";
	std::string target = "";
};

enum LocateObjectID
{
    // Stage
    TestRoom_Model,
    // Player
    Player_Model,
    // Enemy
    RushEnemy_Model,
    ShotEnemy_Model,
    // Box
    WoodBox_Model,
    // Door
    Door_Model,
	DoorOpen_Model,
    // Ball
    Ball_Model,
    // Button
    Button_Model,
    // Pully
    Pully_Model,
	// Goal
	Goal_Model,
};

// jsonにあるTransformをDxlibのVECTORに変換する関数
inline void jsonConvXYZ(const nlohmann::json& j, VECTOR& v)
{
    v.x = j.value("x", 0.0f);
    v.y = j.value("y", 0.0f);
    v.z = j.value("z", 0.0f);
}
// jsonにあるTransformをDxlibのQuaternionに変換する関数
inline void jsonConvRot(const nlohmann::json& j, Quaternion& q)
{
	float x = j.value("x", 0.0f);
	float y = j.value("y", 0.0f);
	float z = j.value("z", 0.0f);
	float w = j.value("w", 1.0f);
	q = { x, y, z, w };
}

// jsonにあるLocateObjectIDをenum LocateObjectIDに変換する関数
inline void from_json(const nlohmann::json& j, GameObject_Unity& obj)
{
	obj.id = j.value("id", 0);
	obj.name = j.value("name", "");
    obj.target = j.value("target", "");
	jsonConvXYZ(j.at("position"),obj.position_3D);
	jsonConvXYZ(j.at("scale"), obj.scale);
	jsonConvRot(j.at("rotation"), obj.rotation);
}