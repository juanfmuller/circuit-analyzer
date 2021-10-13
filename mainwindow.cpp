#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_nextButton_clicked()
{
    QStringList l = ui->leftEdit->displayText().split(" ");
    QStringList r = ui->rightEdit->displayText().split(" ");
    std::vector<std::string> ll;
    std::vector<std::string> rr;
    for (int i = 0; i < l.size(); ++i){
        ll.push_back(l.at(i).toStdString());
    }
    for (int i = 0; i < r.size(); ++i){
        rr.push_back(r.at(i).toStdString());
    }
    c.addComponent(ui->nameEdit->displayText().toStdString(), ll, rr);
}


void MainWindow::on_doneButton_clicked()
{
   QString ph = "";
   std::map<std::string, std::string> nodes = c.findNodes();
   for (auto ii = nodes.begin(); ii != nodes.end(); ++ii) {
       ph = ph + QString::fromStdString((*ii).first + ": ");
       ph = ph + QString::fromStdString((*ii).second);
       ph = ph + "\n";
   }
   ui->outputText->setPlainText(ph);
}

void MainWindow::on_okButton_clicked()
{
    std::string node1 = ui->node1Edit->displayText().toStdString();
    std::string node2 = ui->node2Edit->displayText().toStdString();
    QString component = QString::fromStdString(c.findImpedanceBetweenNodes(node1, node2));
    ui->componentOut->setText(component);
}


void MainWindow::on_plotButton_clicked()
{

    int lowFreq = ui->lowFreqEdit->displayText().toInt();
    int highFreq = ui->highFreqEdit->displayText().toInt();
    int rangeInt = abs(lowFreq) + highFreq;

    QVector<double> x(rangeInt), y(rangeInt);
    int counter = lowFreq;
    double componentValue = ui->componentValue->displayText().toDouble();

    QString w = ui->componentOut->displayText();
    char t;
    for(auto it = w.begin(); it < w.end(); ++it){
        if(*it == 'C'){
            t = 'C';
        }
        if(*it == 'L'){
            t = 'L';
        }
        if(*it == 'R'){
            t = 'R';
        }
    }
    if(t == 'C'){
        for (int i = 0; i < rangeInt; ++i)
        {
          y[i] = 1/(2*3.14*counter*componentValue);
          x[i] = i;
          ++counter;
        }
    } else if (t == 'L'){
        for (int i = 0; i < rangeInt; ++i)
        {
          y[i] = 2*3.14*counter*componentValue;
          x[i] = i;
          ++counter;
        }
    } else {
        for (int i = 0; i < rangeInt; ++i)
        {
          y[i] = componentValue;
          x[i] = i;
          ++counter;
        }
    }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("Frequency");
    ui->customPlot->yAxis->setLabel("Impedance");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(lowFreq, highFreq);
    ui->customPlot->yAxis->setRange(0, 20);
    ui->customPlot->replot();
}

