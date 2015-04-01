/////////////////////////////////////////////////////////////////////////* 
// Filename:		stdafx.h 
// Student Name:	Dau T. Lam
// Class:			CIS 3207 
// Instructor:		TA - Dawei Li / Cody Casey
// Assignment:		Lab 3 - Producer-Consumer Problem  using Win32 API (Run on Windows!)
// Date:			03/29/2015
// 
// Comments: This program will solve the producer-consumer problem using critical 
// section and conditional variables to eliminate any spurious wakeups. Multiple threads
// will be created - one group will be labeled as 'producers' will continously 'produce 
// an item to be deposited' while the other group labeled as 'consumers' will continously
// 'withdraw an item for consumption'. 
// Instructions: to run, compile the main file (.cpp) and execute it using Microsoft Visual Studio
//////////////////////////////////////////////////////////////////////////*/

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include <fstream>


