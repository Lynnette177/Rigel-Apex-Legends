#pragma once
#include"includes.hpp"


uintptr_t localPlayer;
uintptr_t entList;
uintptr_t viewRenderer;
uintptr_t viewMatrix;

float entX = 0;
float entY = 0;

float aX = 0;
float aY = 0;
float lastclosestX = 0;
float lastclosestY = 0;
float entNewVisTime = 0;
float entOldVisTime[100];
int visCooldownTime[100];
int spectatorcount = 0;
int realspeccount = 0;
int timetocountspeczero = 1500;
DWORD timecountnow = 0;
float lastbulletspeed=0;
uintptr_t m_input = 0;