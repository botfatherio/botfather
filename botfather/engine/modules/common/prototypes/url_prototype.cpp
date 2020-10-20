#include "url_prototype.hpp"

#define THIS_URL() qscriptvalue_cast<QUrl>(thisObject())
#define THIS_URL_P() qscriptvalue_cast<QUrl *>(thisObject())

QScriptValue UrlPrototype::constructor(QScriptContext *context,
                                       QScriptEngine *engine) {
    if (context->argumentCount() == 0) {
        return engine->toScriptValue(QUrl());
    }

    if (context->argumentCount() == 1 && context->argument(0).isString()) {
        QString url_string = context->argument(0).toString();
        return engine->toScriptValue(QUrl::fromUserInput(url_string));
    }

    NO_MATCHING_CTOR("Url", URL_PROTOTYPE_DOCS)
}

bool UrlPrototype::isEmpty() const { return THIS_URL().isEmpty(); }

bool UrlPrototype::isValid() const { return THIS_URL().isValid(); }

QString UrlPrototype::getScheme() const { return THIS_URL().scheme(); }

void UrlPrototype::setScheme(const QString &scheme) {
    THIS_URL_P()->setScheme(scheme);
}

QString UrlPrototype::getUsername() const {
    return THIS_URL().userName(QUrl::FullyDecoded);
}

void UrlPrototype::setUsername(const QString &username) {
    THIS_URL_P()->setUserName(username);
}

QString UrlPrototype::getPassword() const {
    return THIS_URL().password(QUrl::FullyDecoded);
}

void UrlPrototype::setPassword(const QString &password) {
    THIS_URL_P()->setPassword(password);
}

QString UrlPrototype::getHost() const {
    return THIS_URL().host(QUrl::FullyDecoded);
}

void UrlPrototype::setHost(const QString &host) { THIS_URL_P()->setHost(host); }

int UrlPrototype::getPort() const { return THIS_URL().port(-1); }

void UrlPrototype::setPort(int port) { THIS_URL_P()->setPort(port); }

QString UrlPrototype::getPath() const {
    return THIS_URL().path(QUrl::FullyDecoded);
}

void UrlPrototype::setPath(const QString &path) { THIS_URL_P()->setPath(path); }

bool UrlPrototype::hasQuery() const { return THIS_URL().hasQuery(); }

QString UrlPrototype::getQuery() const {
    return THIS_URL().query(QUrl::PrettyDecoded);
}

void UrlPrototype::setQuery(const QString &query) {
    THIS_URL_P()->setQuery(query);
}

bool UrlPrototype::hasFragment() const { return THIS_URL().hasFragment(); }

QString UrlPrototype::getFragment() const {
    return THIS_URL().fragment(QUrl::PrettyDecoded);
}

void UrlPrototype::setFragment(const QString &fragment) {
    THIS_URL_P()->setFragment(fragment);
}

QString UrlPrototype::getTopLevelDomain() const {
    return THIS_URL().topLevelDomain(QUrl::FullyDecoded);
}

QString UrlPrototype::getAuthority() const {
    return THIS_URL().authority(QUrl::PrettyDecoded);
}

bool UrlPrototype::isParentOf(const QUrl &child_url) const {
    return THIS_URL().isParentOf(child_url);
}

QUrl UrlPrototype::resolved(const QString &relative) {
    return THIS_URL().resolved(QUrl(relative));
}

QString UrlPrototype::toString() const {
    // Usually toString returns formated information about the object like this:
    // Point(x: 100, y: 100) An url on the other hand has a logical string
    // representation, thus we return it unformated.
    return THIS_URL().toString(QUrl::PrettyDecoded);
}
