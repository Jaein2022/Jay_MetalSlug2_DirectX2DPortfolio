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

//Variable Arguement
#include <cstdarg>

//DirectX11
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>	//MS에서 만든 SIMD연산을 사용하는 함수들을 제공받을 수 있다.	
//SIMD(Single Instruction Multiple Data): 한번의 지시로, 정렬된 여러 값들을 취급하는 연산을 동시에 처리하는 명령어 집합.
//대량의 단순 사칙연산들을 빠르게 처리할 수 있으므로 3D렌더링이나 머신러닝 등을 구현할때 유리해진다.
#include <DirectXCollision.h>
//#include <DirectXCollision.inl>
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI")
