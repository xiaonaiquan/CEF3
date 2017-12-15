#include "include/cef_v8.h"
#include "include/cef_app.h"
class CCefV8HandlerEx :public CefV8Handler
{
public:
	CCefV8HandlerEx(CefRefPtr<CefApp> app);
	~CCefV8HandlerEx();

	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;
public:
	CefRefPtr<CefApp> app;

	IMPLEMENT_REFCOUNTING(CCefV8HandlerEx);

};