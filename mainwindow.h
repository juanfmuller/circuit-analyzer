#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <QString>

class Circuit {
private:
    std::map<std::string, std::vector<std::vector<std::string> > > connectionMap;
public:
    void addComponent(std::string component, std::vector<std::string> leftConn, std::vector<std::string> rightConn) {
        std::vector<std::vector<std::string> > ph;
        ph.push_back(leftConn);
        ph.push_back(rightConn);
        connectionMap[component] = ph;
    }

    void print() {
        for (auto ii = connectionMap.begin(); ii != connectionMap.end(); ++ii) {
            std::cout << (*ii).first << ": ";
            std::vector <std::string> leftVector = (*ii).second[0];
            std::vector <std::string> rightVector = (*ii).second[1];
            for (unsigned j = 0; j < leftVector.size(); j++) {
                std::cout << leftVector[j] << " ";
            }
            std::cout << "| ";
            for (unsigned j = 0; j < rightVector.size(); j++) {
                std::cout << rightVector[j] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::map<std::string, std::string> findNodes() {
        std::map<std::string, std::string> ph;
        int counter = 0;
        for (auto ii = connectionMap.begin(); ii != connectionMap.end(); ++ii) {
            std::string nodeNum = "Node" + std::to_string(counter);
            std::vector <std::string> leftVector = (*ii).second[0];
            std::vector <std::string> rightVector = (*ii).second[1];
            sort(leftVector.begin(), leftVector.end());
            sort(rightVector.begin(), rightVector.end());
            std::string placeholder = (*ii).first;
            if (leftVector.size() > 1) {
                for (unsigned i = 0; i < leftVector.size(); ++i) {
                    placeholder = placeholder + leftVector[i];
                }
                if (ph[nodeNum] != placeholder) {
                    ph[nodeNum] = placeholder;
                    placeholder = (*ii).first;
                    counter++;
                }
            }
            if (rightVector.size() > 1) {
                for (unsigned i = 0; i < rightVector.size(); ++i) {
                    placeholder = placeholder + rightVector[i];
                }
                if (ph[nodeNum] != placeholder) {
                    ph[nodeNum] = placeholder;
                    placeholder = (*ii).first;
                    counter++;
                }
            }
        }
        for (auto it = ph.begin(); it != ph.end(); ++it) {
            std::string sorted = (*it).second;
            sort(sorted.begin(), sorted.end());
            for (auto it2 = ph.begin(); it2 != ph.end(); ) {
                std::string sorted2 = (*it2).second;
                sort(sorted2.begin(), sorted2.end());
                if (sorted2 == sorted && it != it2) {
                    ph.erase(it2++);
                }
                else {
                    ++it2;
                }
            }
        }
        return ph;
    }

    void printNodes() {
        std::map<std::string, std::string> nodes = findNodes();
        for (auto ii = nodes.begin(); ii != nodes.end(); ++ii) {
            std::cout << (*ii).first << ": ";
            std::cout << (*ii).second;
            std::cout << std::endl;
        }
    }

    std::string findImpedanceBetweenNodes(std::string node1, std::string node2) {
        std::map<std::string, std::string> ph = findNodes();
        std::string l = ph[node1];
        std::string k = ph[node2];
        std::string result = "";
        std::cout << l << k << std::endl;
        for (unsigned i = 0; i < l.length(); i+=2) {
            for (unsigned j = 0; j < k.length(); j+=2) {
                if (l[i] == k[j] && l[i + 1] == k[j + 1]) {
                    std::string s(1, k[j]);
                    std::string s1(1, k[j + 1]);
                    result = s + s1;
                    std::cout << k[j] << k[j + 1] << std::endl;
                }
            }
        }
        return result;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_nextButton_clicked();
    void on_doneButton_clicked();
    void on_okButton_clicked();
    void on_plotButton_clicked();

private:
    Ui::MainWindow *ui;
    Circuit c;
};
#endif // MAINWINDOW_H
