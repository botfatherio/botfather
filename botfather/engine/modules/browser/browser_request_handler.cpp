#include "browser_request_handler.hpp"
#include "browser_client.hpp"
#include <QRegularExpression>
#include <QDebug>

BrowserRequestHandler::BrowserRequestHandler(BrowserClient *parent) : m_parent(parent)
{

}

void BrowserRequestHandler::blockResource(const QString &resource_pattern)
{
	m_modified_resources.append({resource_pattern, ""});
}

void BrowserRequestHandler::replaceResource(const QString &resource_pattern, const QString &replacement_url)
{
	m_modified_resources.append({resource_pattern, replacement_url});
}

void BrowserRequestHandler::unmodifyResource(const QString &resource_pattern)
{
	for (int i = 0; i < m_modified_resources.length(); i++)
	{
		QString pattern = m_modified_resources[i].first;
		if (pattern == resource_pattern)
		{
			m_modified_resources.remove(i);
			return;
		}
	}
}

void BrowserRequestHandler::unmodifyResources()
{
	m_modified_resources.clear();
}

void BrowserRequestHandler::setUserAgent(const QString &user_agent)
{
    m_user_agent_override = user_agent;
}

void BrowserRequestHandler::resetUserAgent()
{
    m_user_agent_override.clear();
}

CefRequestHandler::ReturnValue BrowserRequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
{
    QString url = QString::fromStdString(request->GetURL().ToString());

    // Modify user agent
    if (!m_user_agent_override.isEmpty()) {
        std::multimap<CefString, CefString> header_map;
        request->GetHeaderMap(header_map);
        std::multimap<CefString, CefString>::iterator user_agent_match = header_map.find("User-Agent");
        if (user_agent_match != header_map.end())
        {
        header_map.erase(user_agent_match);
        }
        header_map.insert({"User-Agent", m_user_agent_override.toStdString()});
        request->SetHeaderMap(header_map);
    }

    // Modify resources
	for (int i = 0; i < m_modified_resources.length(); i++)
	{
		QString pattern = m_modified_resources[i].first;
		QRegularExpression regex(pattern);

		if (!regex.match(url).hasMatch())
		{
			continue;
		}

		QString new_url = m_modified_resources[i].second;
		if (new_url.isEmpty())
		{
			qDebug() << "Blocking resource" << url;
			return RV_CANCEL; // Block the resource from loading.
		}

		qDebug() << "Replacing resource" << url << "with" << new_url;
		request->SetURL(new_url.toStdString());
	}

	return RV_CONTINUE;
}

void BrowserRequestHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path)
{
	qDebug() << "Plugin Crashed:" << QString::fromStdString(std::string(plugin_path));
	emit m_parent->pluginCrashedSignal();
}

void BrowserRequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status)
{
	switch (status) {
	case TS_ABNORMAL_TERMINATION:
		qDebug() << "Renderer Terminated. Non-zero exit status.";
		break;
	case TS_PROCESS_WAS_KILLED:
		qDebug() << "Renderer Was Killed. SIGKILL or task manager kill.";
		break;
	case TS_PROCESS_CRASHED:
		qDebug() << "Renderer Crashed. Segmentation fault.";
		break;
	default:
		qDebug() << "Abnormal CEF termination happened." << status;
		break;
	}
	emit m_parent->rendererCrashedSignal();
}
