#include "mainwindow.h"
#include <ossia/network/zeroconf/zeroconf.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , m_browser{}
  , m_layout{&m_main}
  , m_serverList{&m_main}
  , m_model{this}
  , m_view{&m_main}
{
  this->setCentralWidget(&m_main);
  m_layout.addWidget(new QLabel{"Servers"}, 0, 0);
  m_layout.addWidget(&m_serverList, 1, 0);
  m_layout.addWidget(new QLabel{"Content"}, 0, 1);
  m_layout.addWidget(&m_view, 1, 1);
  m_layout.setSpacing(5);

  m_mgr.setStrictTransportSecurityEnabled(false);
  m_view.setModel(&m_model);
  connect(&m_browser, &ZeroconfBrowser::serversChanged,
          this, [=] (const auto& servers) {
    m_serverList.clear();
    m_serverList.addItems(servers);
  });

  connect(&m_serverList, &QListWidget::currentTextChanged,
          this, [=] (const auto& server) {
    if(server.isEmpty())
      return;

    auto reply = m_mgr.get(QNetworkRequest{server + "/"});
    connect(reply, &QNetworkReply::readyRead,
            this, [=] {
      m_model.loadJson(reply->readAll());
    });

  });
}

MainWindow::~MainWindow()
{

}

ZeroconfBrowser::ZeroconfBrowser()
{
  static int timer = 0;
  moveToThread(&m_zeroConfThread);
  connect(&m_zeroConfThread, &QThread::started,
          this, [=] {
    timer = startTimer(5000);
  });
  connect(&m_zeroConfThread, &QThread::finished,
          this, [=] {
    killTimer(timer);
  });
  m_zeroConfThread.start();
}

ZeroconfBrowser::~ZeroconfBrowser()
{
  m_zeroConfThread.exit(0);
  m_zeroConfThread.wait();
}

void ZeroconfBrowser::timerEvent(QTimerEvent* event)
{
  auto lst = ossia::net::list_oscquery_devices();
  QStringList list;
  for(auto e : lst)
  {
    list += QString::fromStdString("http://" + e.host + ":" + std::to_string(e.port));
  }

  emit serversChanged(list);
}

