#pragma once
#include <QtWidgets>
#include <QtNetwork>
#include <QThread>
#include <QJsonModel/qjsonmodel.h>
// Should allow to :
// * browse existing OSCQuery devices
// * send requests
// * show responses

class ZeroconfBrowser : public QObject
{
  Q_OBJECT
public:
  ZeroconfBrowser();
  ~ZeroconfBrowser() override;

  Q_SIGNAL void serversChanged(QStringList);
private:
  void timerEvent(QTimerEvent* event) override;
  QThread m_zeroConfThread;

};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  ZeroconfBrowser m_browser;
  QNetworkAccessManager m_mgr;

  QWidget m_main;
  QGridLayout m_layout;
  QListWidget m_serverList;

  QJsonModel m_model;
  QTreeView m_view;
};
