﻿/*
* IEAC_U07.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a EAC_U07 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: ECD (Equipment Command) repeating</li>
///<li>3: ROL (Role) optional </li>

#ifndef IEAC_U07_H
#define IEAC_U07_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IECD.h"
#include "IROL.h"

class IEAC_U07 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void AddECD(IECD* ecd) = 0;

	virtual void SetROL(IROL* rol) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual bool GetECD(IECD* ecd, std::size_t index) = 0;

	virtual bool GetROL(IROL* rol) = 0;

	virtual int GetECDSize() = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IEAC_U07* CreateEacU07();
#else
IEAC_U07* CreateEacU07();
#endif

#endif


