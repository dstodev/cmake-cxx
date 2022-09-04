#include <QApplication>
#include <QtWidgets>

using namespace project;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QPushButton button("Whoa!");
	button.show();

	return app.exec();
}
