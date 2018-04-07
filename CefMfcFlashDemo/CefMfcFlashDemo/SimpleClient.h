#pragma once

#include "include/cef_client.h"
#include "include/cef_app.h"

class CSimpleClient : public CefClient, public CefLifeSpanHandler, public CefApp, public CefBrowserProcessHandler
{
public:
	CSimpleClient();
	~CSimpleClient();

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
	{ return this; }

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override
	{
		return this;
	}

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	CefRefPtr<CefBrowser> GetBrowser() { return m_cefBrowser; }

	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	
private:
	CefRefPtr<CefBrowser> m_cefBrowser;

	IMPLEMENT_REFCOUNTING(CSimpleClient);
};