#include "CEFV8HandlerEx.h"
#include <tchar.h>


CCefV8HandlerEx::CCefV8HandlerEx(CefRefPtr<CefApp> app)
{
	this->app = app;
}

CCefV8HandlerEx::~CCefV8HandlerEx()
{

}

bool CCefV8HandlerEx::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{

	if (name == "NativeLogin")
	{
		if (arguments.size() == 2)
		{
			
			CefString strUser = arguments.at(0)->GetStringValue();
			CefString strPassword = arguments.at(1)->GetStringValue();

			TCHAR szLog[256] = { 0 };
			_stprintf_s(szLog, 256, L"user - %s, password - %s\r\n", strUser.c_str(), strPassword.c_str());
			
			OutputDebugString(szLog);

			////TODO: doSomething() in native way
			//
			//retval = CefV8Value::CreateInt(0);
			//MessageBox(NULL, szLog, L"ssss", NULL);
			
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("login_msg");

			// Retrieve the argument list object.
			CefRefPtr<CefListValue> args = msg->GetArgumentList();

			// Populate the argument values.
			args->SetSize(2);
			args->SetString(0, strUser);
			args->SetString(1, strPassword);

			// Send the process message to the browser process.
			CefV8Context::GetCurrentContext()->GetBrowser()->SendProcessMessage(PID_BROWSER, msg);

		}
		else
		{
			retval = CefV8Value::CreateInt(1);
		}

		return true;
	}
	else if (name == "GetId")
	{
		if (arguments.size() == 0)
		{
			// execute javascript 
			// just for test
			OutputDebugString(L"askdhjakshdkjahskjdhakjshdkjahsdkjah");
			
			CefRefPtr<CefFrame> frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
			frame->ExecuteJavaScript("alert('Hello, I came from native world.')", frame->GetURL(), 0);

			// return to JS
			retval = CefV8Value::CreateString("asdasd");
			return true;
		}
	}
	// Function does not exist.
	return false;
}

