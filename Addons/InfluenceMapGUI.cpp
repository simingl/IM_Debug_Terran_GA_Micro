/////////////////////////////////
//MainDlg.cpp
////////////////////////////////
                                                              
#include "InfluenceMapGUI.h"                                                               

//ȫ��ָ��,����ָ���������Ķ���,��this��ָ�����                                                              
InfluenceMapGUI *g_pDlg = NULL;                         


//���캯��                                                                               
InfluenceMapGUI::InfluenceMapGUI(HINSTANCE hInst)                                                
{                                                                                  
	g_pDlg = this; //�洢this����ȫ�ֱ�����                                                                   
	m_hInst = hInst; 
	std::string text = "->influenceMapGUI Construction";
	BWAPI::Broodwar->printf("%s",text.c_str());
}                                                                                  

//��������                                                                               
InfluenceMapGUI::~InfluenceMapGUI()                                                              
{                                                                                  

}                                                                                  

//�����Ի���                                                                           
BOOL InfluenceMapGUI::CreateMainDlg()                                                     
{          
	std::string text = "->influenceMapGUI CreateDlg";
	BWAPI::Broodwar->printf("%s",text.c_str());

	m_hDlg = CreateDialog(m_hInst,MAKEINTRESOURCE("IM"),NULL,NULL);   
	if(m_hDlg == NULL)                                                               
	{                                                                                
		return FALSE;                                                                  
	}                                                                                
	ShowWindow(m_hDlg,TRUE);//��ʾ����                                                         

	return TRUE;                                                                     
}                                                                                  



