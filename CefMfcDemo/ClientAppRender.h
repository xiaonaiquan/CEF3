#include "include/cef_app.h"
#include "include/cef_client.h"
//cef_v8.h
#include "include/cef_v8.h"
#include "CEFV8HandlerEx.h"

class ClientAppRender : public CefApp,public CefRenderProcessHandler
{
public:
	ClientAppRender();
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
	{
		return this;
	}

	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	void OnContextCreated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context);

	void OnWebKitInitialized() OVERRIDE;

	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;
	
private:
	//CefRefPtr<CCefV8HandlerEx> m_v8Handler;

	IMPLEMENT_REFCOUNTING(ClientAppRender);

private:

};



