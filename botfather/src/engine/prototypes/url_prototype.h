#ifndef BFP_ENGINE_PROTOTYPES_URL_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_URL_PROTOTYPE_H
#define URL_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/url/"

#include "abstract_prototype.h"
#include <QUrl>

Q_DECLARE_METATYPE(QUrl)
Q_DECLARE_METATYPE(QUrl*)

// ftp://uname:pword@sharing.example.com:2021
// https://sub.example.com/contact/me/?something=cool&color=blue#fragment
// scheme: ftp
// username: uname
// password: pword
// host: ftp.example.com
// port: 2021
// query: something=cool&color=blue
// fragment: fragment
// topleveldomain: .com
// authority: uname:pword@sharing.example.com:2021

class UrlPrototype : public AbstractPrototype
{
	Q_OBJECT

public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	// Returns true if the URL has no data; otherwise returns false.
	// http://doc.qt.io/qt-5/qurl.html#isEmpty
	Q_INVOKABLE bool isEmpty() const;

	// Returns true if the URL is non-empty and valid; otherwise returns false.
	// http://doc.qt.io/qt-5/qurl.html#isValid
	Q_INVOKABLE bool isValid() const;

	// Returns the scheme of the URL. If an empty string is returned, this means the scheme is undefined and the URL is then relative.
	// http://doc.qt.io/qt-5/qurl.html#scheme
	Q_INVOKABLE QString getScheme() const;

	// Sets the scheme of the URL to scheme.
	// http://doc.qt.io/qt-5/qurl.html#setScheme
	Q_INVOKABLE void setScheme(const QString &scheme);

	// Returns the user name of the URL if it is defined; otherwise an empty string is returned.
	// http://doc.qt.io/qt-5/qurl.html#userName
	Q_INVOKABLE QString getUsername() const;

	// Sets the URL's user name to username.
	// http://doc.qt.io/qt-5/qurl.html#setUserName
	Q_INVOKABLE void setUsername(const QString &username);

	// Returns the password of the URL if it is defined; otherwise an empty string is returned.
	// http://doc.qt.io/qt-5/qurl.html#password
	Q_INVOKABLE QString getPassword() const;

	// Sets the URL's password to password.
	// http://doc.qt.io/qt-5/qurl.html#setPassword
	Q_INVOKABLE void setPassword(const QString &password);

	// Returns the host of the URL if it is defined; otherwise an empty string is returned.
	// http://doc.qt.io/qt-5/qurl.html#host
	Q_INVOKABLE QString getHost() const;

	// Sets the host of the URL to host. The host is part of the authority.
	// http://doc.qt.io/qt-5/qurl.html#setHost
	Q_INVOKABLE void setHost(const QString &host);

	// Returns the port of the URL, or -1 if the port is unspecified.
	// http://doc.qt.io/qt-5/qurl.html#port
	Q_INVOKABLE int getPort() const;

	// Sets the port of the URL to port.
	// http://doc.qt.io/qt-5/qurl.html#setPort
	Q_INVOKABLE void setPort(int port);

	// Returns the path of the URL.
	// http://doc.qt.io/qt-5/qurl.html#path
	Q_INVOKABLE QString getPath() const;

	// Sets the path of the URL to path. The path is the part of the URL that comes after the authority but before the query string.
	// http://doc.qt.io/qt-5/qurl.html#setPath
	Q_INVOKABLE void setPath(const QString &path);

	// Returns true if this URL contains a Query (i.e., if ? was seen on it).
	// http://doc.qt.io/qt-5/qurl.html#hasQuery
	Q_INVOKABLE bool hasQuery() const;

	// Returns the query string of the URL if there's a query string, or an empty result if not.
	// http://doc.qt.io/qt-5/qurl.html#query
	Q_INVOKABLE QString getQuery() const;

	// Sets the query string of the URL to query.
	// http://doc.qt.io/qt-5/qurl.html#setQuery
	Q_INVOKABLE void setQuery(const QString &query);

	// Returns true if this URL contains a fragment (i.e., if # was seen on it).
	// http://doc.qt.io/qt-5/qurl.html#hasFragment
	Q_INVOKABLE bool hasFragment() const;

	// Returns the fragment of the URL.
	// http://doc.qt.io/qt-5/qurl.html#fragment
	Q_INVOKABLE QString getFragment() const;

	// Sets the fragment of the URL to fragment.
	// http://doc.qt.io/qt-5/qurl.html#setFragment
	Q_INVOKABLE void setFragment(const QString &fragment);

	// Returns the TLD (Top-Level Domain) of the URL, (e.g. .co.uk, .net).
	// http://doc.qt.io/qt-5/qurl.html#topLevelDomain
	Q_INVOKABLE QString getTopLevelDomain() const;

	// Returns the authority of the URL if it is defined; otherwise an empty string is returned.
	// http://doc.qt.io/qt-5/qurl.html#authority
	Q_INVOKABLE QString getAuthority() const;

	// Returns true if this URL is a parent of childUrl.
	// http://doc.qt.io/qt-5/qurl.html#isParentOf
	Q_INVOKABLE bool isParentOf(const QUrl &child_url) const;

	// Returns the result of the merge of this URL with relative. This URL is used as a base to convert relative to an absolute URL.
	// http://doc.qt.io/qt-5/qurl.html#resolved
	Q_INVOKABLE QUrl resolved(const QUrl &relative);

	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_URL_PROTOTYPE_H
