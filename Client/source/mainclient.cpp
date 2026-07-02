#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QWidget *parent, boost::asio::io_context &io)
    : QMainWindow(parent)
    , ui(new Ui::MainClient)
    , plot_{std::make_unique<QCustomPlot>(this)}
{
    ui->setupUi(this);

    ui->plot->addWidget(plot_.get());

    plot_->addGraph();
    // plot_->addGraph();

    QVector<unsigned> data = parseRcvFile("/Users/alekseypodoplelov/Downloads/1rcvdata.txt");
    qDebug() << "data_size=" << data.size();

    QVector<double> sin, x, cos, amp;

    for(size_t i{};i<data.size();++i) {
        x.push_back(i);
        sin.push_back(static_cast<short>(data[i] & 0xFFFF));
        cos.push_back(static_cast<short>((data[i] >> 16) & 0xFFFF));
        amp.push_back(static_cast<float>(sqrt(10.0)*sqrt(sin[i] * sin[i]/10.0 + cos[i] * cos[i]/10.0)));
    }
    plot_->graph(0)->setData(x, amp);
    plot_->graph(0)->setPen(QPen(Qt::blue));
    // plot_->graph(1)->setData(x, cos);
    // plot_->graph(1)->setPen(QPen(Qt::red));
    plot_->rescaleAxes();
    plot_->replot();

    QObject::connect(ui->qpsh_connect, &QPushButton::clicked, this, &MainClient::slot_connect);
    QObject::connect(ui->qpsh_disconnect, &QPushButton::clicked, this, &MainClient::slot_disconnect);
    QObject::connect(ui->qpsh_send, &QPushButton::clicked, this, &MainClient::slot_send);

    if( !Iserver_ ) {
        Iserver_ = std::make_shared<BoostConnection>(io);
    }
}

MainClient::~MainClient()
{
    delete ui;
}

void MainClient::slot_connect()
{
    QString host = ui->qlineedit_host->text();
    std::cout << "trying connect. addr:" << host.toStdString() << std::endl;
    Iserver_->connect(host.toStdString(), proto_project::kServerPort);
}

void MainClient::slot_disconnect()
{
    Iserver_->disconnect();
}

void MainClient::slot_send()
{
    tcp_data::TestStruct _struct;
    _struct.a = "test";
    _struct.b = 1;
    _struct.c = 0.75;
    _struct.d.reserve(5);
    for( size_t i=0;i<5;++i )
        _struct.d.push_back(i+i*i);

    Iserver_->async_write(proto_project::dpt::TestStruct, _struct.serilize());
}
