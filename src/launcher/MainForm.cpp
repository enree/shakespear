#include "MainForm.h"

#include "TestRunner.h"
#include "ui_MainForm.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextBrowser>

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

    connect(
        m_testRunner.get(),
        &TestRunner::message,
        m_ui->log,
        &QTextBrowser::append);

    connect(m_ui->execute, &QPushButton::clicked, m_testRunner.get(), [this]() {
        const QString script = m_ui->scriptEditor->toPlainText();
        m_testRunner->runTestCase(script);
    });
}

MainForm::~MainForm() = default;

} // namespace shakespear
