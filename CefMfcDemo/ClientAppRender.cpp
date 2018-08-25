#include "ClientAppRender.h"
#include <tchar.h>

ClientAppRender::ClientAppRender()
{
	//m_v8Handler = new CCefV8HandlerEx;
}


void ClientAppRender::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context)
{
	OutputDebugString(L"ClientAppRenderer::OnContextCreated, create window binding\r\n");

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();


	// Create the "NativeLogin" function.
	CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("NativeLogin", new CCefV8HandlerEx(this));

	// Add the "NativeLogin" function to the "window" object.
	object->SetValue("NativeLogin", func, V8_PROPERTY_ATTRIBUTE_NONE);
}


void ClientAppRender::OnWebKitInitialized()
{
	OutputDebugString(L"ClientAppRenderer::OnWebKitInitialized, create js extensions\r\n");
	std::string app_code =
		"var app;"
		"if (!app)"
		"    app = {};"
		"(function() {"
		"    app.GetId = function() {"
		"        native function GetId();"
		"        return GetId();"
		"    };"
		"})();";
	CefRegisterExtension("v8/app", app_code, new CCefV8HandlerEx(this));
	
}

bool ClientAppRender::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	const std::string& messageName = message->GetName();
	if (messageName == "login_reply")
	{
		// extract message
		CefRefPtr<CefListValue> args = message->GetArgumentList();
		int status = args->GetInt(0);
		/*OutputDebugString(status == 0 ? L"Renderer process, login ok\r\n" : L"Renderer process, login failed\r\n");
		OutputDebugString(L"chenggong\r\n");*/
		TCHAR szLog[256] = { 0 };
		//_stprintf_s(szLog, 256, L"BrowserProcess, user - %s, password - %s\r\n", strUser.c_str(), strPassword.c_str());
		if (status == 0)
		{
			_stprintf_s(szLog, 256, L"Renderer process, login ok\r\n");
		}
		else
		{
			_stprintf_s(szLog, 256, L"Renderer process, login failed\r\n");
		}
		OutputDebugString(szLog);
		CefRefPtr<CefFrame> frame = browser->GetMainFrame();
		frame->ExecuteJavaScript("alert('Got Login Reply from Browser process')", frame->GetURL(), 0);
		return true;
	}
	return false;
}

void ClientAppRender::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
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








