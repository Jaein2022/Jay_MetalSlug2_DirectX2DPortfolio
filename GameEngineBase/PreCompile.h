#pragma once

//System Basement
#include <assert.h>
#include <Windows.h>
#include <chrono>
#include <algorithm>
#include <functional>
#include <iostream>

//IO
#include <filesystem>

//Mathmatics
#include <random>
#include <math.h>
#include <float.h>
#include <ratio>

//Containers
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <unordered_map>

//Variable Arguement
#include <cstdarg>

//DirectX11
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>	//MS���� ���� SIMD������ ����ϴ� �Լ����� �������� �� �ִ�.	
//SIMD(Single Instruction Multiple Data): �ѹ��� ���÷�, ���ĵ� ���� ������ ����ϴ� ������ ���ÿ� ó���ϴ� ��ɾ� ����.
//�뷮�� �ܼ� ��Ģ������� ������ ó���� �� �����Ƿ� 3D�������̳� �ӽŷ��� ���� �����Ҷ� ����������.
#include <DirectXCollision.h>
//#include <DirectXCollision.inl>
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI")
