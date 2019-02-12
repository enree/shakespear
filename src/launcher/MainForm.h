#pragma once

#include <QWidget>

#include <memory>

namespace Ui
{
class MainForm;
}

namespace shakespear
{

class TestRunner;

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(
        std::shared_ptr<TestRunner> testRunner, QWidget* parent = nullptr);
    ~MainForm();

private:
    std::unique_ptr<Ui::MainForm> m_ui;
    std::shared_ptr<TestRunner> m_testRunner;
};

} // namespace shakespear
