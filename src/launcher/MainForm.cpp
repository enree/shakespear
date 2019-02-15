#include "MainForm.h"

#include "TestRunner.h"
#include "ui_MainForm.h"

#include <QPushButton>

namespace shakespear
{

MainForm::MainForm(std::shared_ptr<TestRunner> testRunner, QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::MainForm)
    , m_testRunner(std::move(testRunner))
{
    m_ui->setupUi(this);

    connect(
        m_ui->run,
        &QPushButton::clicked,
        m_testRunner.get(),
        &TestRunner::runTestSuite);

    connect(
        m_ui->stop,
        &QPushButton::clicked,
        m_testRunner.get(),
        &TestRunner::stopTestSuite);
}

MainForm::~MainForm() {}

} // namespace shakespear
