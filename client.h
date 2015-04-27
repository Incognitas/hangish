/*

Hanghish
Copyright (C) 2015 Daniele Rogora

This file is part of Hangish.

Hangish is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Hangish is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Nome-Programma.  If not, see <http://www.gnu.org/licenses/>

*/


#ifndef CLIENT_H
#define CLIENT_H

#include <QDateTime>

#include "authenticator.h"
#include "channel.h"
#include "rostermodel.h"
#include "conversationmodel.h"
#include "contactsmodel.h"
#include <QCryptographicHash>
#include "utils.h"
#include "notifier.h"

class MessageField;


class Client : public QObject
{
    Q_OBJECT

    Authenticator *auth;
    QNetworkAccessManager *nam;
    QNetworkCookieJar cJar;
    QList<QNetworkCookie> sessionCookies;
    InitialData iData;
    QString api_key, header_date, header_version, header_id, channel_path, clid, channel_ec_param, channel_prop_param, sync_timestamp;
    User myself;
    Channel *channel;
    QNetworkCookie pvt;

private:
    Notifier *notifier;
    bool needLogin;
    bool needSync;
    QDateTime needSyncTS;
    QDateTime lastSetActive;
    QList<OutgoingImage> outgoingImages;

    void sendImageMessage(QString convId, QString imgId, QString segments);
    void performImageUpload(QString url);
    bool appPaused;
    void forceChannelCheckAndRestore();
    bool initCompleted;
    User parseMySelf(const QString &sreply);
    void getPVTToken();
    User getUserById(QString chatId);
    User parseEntity(QList<MessageField> entity);
    QList<User> parseClientEntities(QList<MessageField> entities);
    QList<User> parseGroup(QList<MessageField> group);
    QList<User> users;
    QList<User> parseUsers(const QString &userString);
    Conversation getConvById(QString cid);

    RosterModel *rosterModel;
    ConversationModel *conversationModel;
    ContactsModel *contactsModel;

    QString getRequestHeader();
    void parseSelfConversationState(QList<MessageField> stateFields, Conversation& res);
    void parseConversationAbstract(QList<MessageField> abstractFields, Conversation& res);
    Conversation parseConversation(QList<MessageField> conversation);
    void parseConversationState(MessageField conv);
    void parseConversationLog(MessageField conv);
    QList<Conversation> parseConversations(const QString &conv);
    void followRedirection(QUrl url);

    QByteArray getAuthHeader();
    QNetworkReply * sendRequest(QString function, QString json);
    void syncAllNewEvents(QDateTime timestamp);

public:
    Client(RosterModel *prosterModel, ConversationModel *pconversationModel, ContactsModel *pcontactsModel);
    void initChat(QString pvt);
    Q_INVOKABLE void sendChatMessage(QString segments, QString conversationId);
    Q_INVOKABLE void sendImage(QString segments, QString conversationId, QString filename);
    Q_INVOKABLE QString getSelfChatId();
    Q_INVOKABLE void sendCredentials(QString uname, QString passwd);
    Q_INVOKABLE void send2ndFactorPin(QString pin);
    Q_INVOKABLE void deleteCookies();
    Q_INVOKABLE void testNotification();
    Q_INVOKABLE void forceChannelRestore();
    Q_INVOKABLE void setActiveClient();
    Q_INVOKABLE void setFocus(QString convId, int status);
    Q_INVOKABLE void setTyping(QString convId, int status);
    Q_INVOKABLE void setPresence(bool goingOffline);
    Q_INVOKABLE void setAppPaused();
    Q_INVOKABLE void setAppOpened();
    Q_INVOKABLE QString getLastIncomingConversationId();
    Q_INVOKABLE QString getLastIncomingConversationName();
    Q_INVOKABLE void retrieveConversationLog(QString convId);




public slots:
    Q_INVOKABLE void updateWatermark(QString convId);
    void authenticationDone();
    void initDone();
    void networkReply();
    void postReply(QNetworkReply *reply);
    void sendMessageReply();
    void uploadImageReply();
    void uploadPerformedReply();
    void syncAllNewEventsReply();
    //void syncAllNewEventsDataArrval();
    void setActiveClientReply();
    void setTypingReply();
    void setPresenceReply();
    void updateWatermarkReply();
    void pvtReply();
    void channelLostSlot();
    void channelRestoredSlot(QDateTime lastRec);
    void connectivityChanged(QString a,QDBusVariant b);
    void isTypingSlot(QString convId, QString chatId, int type);
    void authFailedSlot(QString error);
    void loginNeededSlot();
    void updateClientId(QString newID);
    void setFocusReply();
    void cookieUpdateSlot(QNetworkCookie cookie);
    void qnamUpdatedSlot(QNetworkAccessManager *qnam);
    void catchNotificationForCover(int num);
    void catchDeletedNotifications();
    void retrieveConversationLogReply();

signals:
    void loginNeeded();
    void messageSent();
    void messageNotSent();
    void conversationLoaded();
    void initFinished();
    void channelLost();
    void channelRestored();
    void isTyping(QString convid, QString uname, int status);
    void showNotification(QString preview, QString summary, QString body, QString sender, int num);
    void authFailed(QString error);
    void showNotificationForCover(int num);
    void deletedNotifications();

};

#endif // CLIENT_H
