#pragma once
#include <raylib.h>
class Generator
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ApplySettings() = 0;
	virtual void ResetSettings() = 0;
	virtual Texture2D GetImage() = 0;
};