/////////////////////////////////
//MainDlg.cpp
////////////////////////////////
                                                              
#include "InfluenceMapGUI.h"                                                               

//全局指针,用来指向所创建的对象,即this的指向对象                                                              
InfluenceMapGUI *g_pDlg = NULL;                         


//构造函数                                                                               
InfluenceMapGUI::InfluenceMapGUI(HINSTANCE hInst)                                                
{                                                                                  
	g_pDlg = this; //存储this对象到全局变量中                                                                   
	m_hInst = hInst; 
	std::string text = "->influenceMapGUI Construction";
	BWAPI::Broodwar->printf("%s",text.c_str());
}                                                                                  

//析构函数                                                                               
InfluenceMapGUI::~InfluenceMapGUI()                                                              
{                                                                                  

}                                                                                  

//创建对话框                                                                           
BOOL InfluenceMapGUI::CreateMainDlg()                                                     
{          
	std::string text = "->influenceMapGUI CreateDlg";
	BWAPI::Broodwar->printf("%s",text.c_str());

	m_hDlg = CreateDialog(m_hInst,MAKEINTRESOURCE("IM"),NULL,NULL);   
	if(m_hDlg == NULL)                                                               
	{                                                                                
		return FALSE;                                                                  
	}                                                                                
	ShowWindow(m_hDlg,TRUE);//显示窗口                                                         

	return TRUE;                                                                     
}                                                                                  



