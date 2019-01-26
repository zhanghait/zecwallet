#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "mainwindow.h"
#include "precompiled.h"


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WSServer : public QObject
{
    Q_OBJECT
public:
    explicit WSServer(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~WSServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    MainWindow *m_mainWindow;
    QList<QWebSocket *> m_clients;
    bool m_debug;
};

enum NonceType {
    LOCAL = 1,
    REMOTE
};

class AppDataServer {
public:
    static void          connectAppDialog(QWidget* parent);

    static QJsonDocument processSendTx(QJsonObject sendTx, MainWindow* mainwindow);
    static QJsonDocument processMessage(QString message, MainWindow* mainWindow);
    static QJsonDocument processDecryptedMessage(QString message, MainWindow* mainWindow);
    static QJsonDocument processGetInfo(MainWindow* mainWindow);
    static QJsonDocument processGetTransactions(MainWindow* mainWindow);

    static QString       decryptMessage(QJsonDocument msg);
    static QString       encryptOutgoing(QString msg);

    static QList<QString>       getSecretHex();
    static void                 saveNewSecret(QString secretHex);

    static QString       getNonceHex(NonceType nt);
    static void          saveNonceHex(NonceType nt, QString noncehex);
};

class AppDataModel {
public:
    static AppDataModel* getInstance() {
        if (instance == NULL)
            instance = new AppDataModel();

        return instance;
    }

    double getTBalance()     { return balTransparent;  }
    double getZBalance()     { return balShielded; }
    double getTotalBalance() { return balTotal; }

    void   setBalances(double transparent, double shielded) {
        balTransparent = transparent;
        balShielded = shielded;
        balTotal = balTransparent + balShielded;
    }

private:
    AppDataModel() = default;   // Private, for singleton

    double balTransparent;
    double balShielded;
    double balTotal;
    double balMaxSingle;

    QString saplingAddress;

    static AppDataModel* instance;
};

#endif // WEBSOCKETS_H