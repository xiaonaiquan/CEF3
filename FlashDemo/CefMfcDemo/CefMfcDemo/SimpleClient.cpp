#include "SimpleClient.h"


CSimpleClient::CSimpleClient()
{
}


CSimpleClient::~CSimpleClient()
{
}

void CSimpleClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	m_cefBrowser = browser;

}

void CSimpleClient::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{

	command_line->AppendSwitch("no-proxy-server");
	command_line->AppendSwitch("--enable-npapi");
	command_line->AppendSwitch("--disable-web-security");
	command_line->AppendSwitch("allow-outdated-plugins");
	
	//manifest.json中的version
	command_line->AppendSwitchWithValue("ppapi-flash-version", "20.0.0.228");

	//加载flash插件
	command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");

}


