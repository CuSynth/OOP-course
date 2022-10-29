#include <iostream>
#include <cstring>

#include "interface.hpp"

//
// todo:
// - � optimizer'� ����������� �� ��� ���� R' -> R          (!!!)
// - ���� �� �������� ������� (���������� ��� Process-�)	(!!)
// - ��� ����������� �������� CFOP-�������
//
// - ��������� � �����, ��� �� ����������� ������� ������ � ����� ��������� �������
// - ���������� ������������� ����, ����� �������� � ����� ������
//

const char* WRONG_PARAM_MSG = "Wrong param! Use 'help' to see help menu. ";

int main(int argc, char** argv) {

    try {
        if (argc > 2) {
            std::cout << WRONG_PARAM_MSG << std::endl;
        }
        else {
            interfacer itfs;

            if (argc == 1) {
                itfs.printMenu();

                while (true) {
                    if (con_keyPressed())
                        if (itfs.interrupt(con_getKey()))
                            break;
                }
            }
            else {
                if (!strcmp(argv[1], "rand")) {
                    itfs.rndProcess();
                }
                else if (!strcmp(argv[1], "solve")) {
                    itfs.solverProcess();
                }
                else if (!strcmp(argv[1], "demo")) {
                    itfs.demoProcess();
                }
                else if (!strcmp(argv[1], "help")) {
                    itfs.helpProcess();
                }
                else
                    std::cout << WRONG_PARAM_MSG << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cout << e.what();
        return 1;
    }

    return 0;
}