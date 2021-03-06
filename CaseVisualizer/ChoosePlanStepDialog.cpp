#include "ChoosePlanStepDialog.h"

#include <QStringList>
#include <QKeyEvent>
#include <QCompleter> 
#include <QMessageBox>

#ifndef TOOLBOX_H
#include "Toolbox.h"
#endif
#ifndef METADATA_H
#include "MetaData.h"
#endif
using namespace IStrategizer;

ChoosePlanStepDialog::ChoosePlanStepDialog(CrossMap<unsigned, string>* p_idLookup, bool p_goals, bool p_actions, QWidget *parent)
: QDialog(parent, Qt::FramelessWindowHint)
{
    ui.setupUi(this);
    m_idLookup = p_idLookup;
    InitializePlanStepList(p_goals, p_actions);
}
//----------------------------------------------------------------------------------------------
void ChoosePlanStepDialog::InitializePlanStepList(bool p_goals, bool p_actions)
{
    QStringList planStepList;
    if(p_goals)
    {
        for(unsigned i = START(GoalType); i < END(GoalType); ++i)
        {
            string goalName;
            
            if (m_idLookup->TryGetByFirst(i, goalName))
            {
                planStepList << QString::fromLocal8Bit(goalName.c_str());
            }
            else
            {
                string errorMsg = "Key '";
                errorMsg += i;
                errorMsg += "' not found";

                QMessageBox::warning(this, tr("Initialization Error"), tr("Key '%1' not found in the ID lookup file").arg(i));
            }
        }
    }

    if(p_actions)
    {
        for(unsigned i = START(ActionType); i < END(ActionType); ++i)
        {
            string actionName;
            
            if (m_idLookup->TryGetByFirst(i, actionName))
            {
                planStepList << QString::fromLocal8Bit(actionName.c_str());
            }
            else
            {
                QMessageBox::warning(this, tr("Initialization Error"), tr("Key '%1' not found in the ID lookup file").arg(i));
            }
        }
    }

    m_autoComplete = new QCompleter(planStepList);
    ui.comboBox->setCompleter(m_autoComplete);
    ui.comboBox->setEditable(true);
    ui.comboBox->addItems(planStepList);
}
//----------------------------------------------------------------------------------------------
void ChoosePlanStepDialog::on_comboBox_currentIndexChanged(QString p_item)
{
    std::string sItem = p_item.toLocal8Bit();

    if (m_idLookup->TryGetBySecond(sItem, m_selectedPlanStepId))
    {
        if(BELONG(GoalType, m_selectedPlanStepId))
        {
            ui.lblPlanStepType->setText("Goal");
        }
        else
        {
            ui.lblPlanStepType->setText("Action");
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Initialization Error"), tr("Key '%1' not found in the ID lookup file").arg(sItem.c_str()));
    }
}
//----------------------------------------------------------------------------------------------
void ChoosePlanStepDialog::on_btnOK_clicked()
{
    string selectedItem = ui.comboBox->currentText().toLocal8Bit();
    m_selectedPlanStepId = m_idLookup->GetBySecond(selectedItem);

    this->accept();
}
//----------------------------------------------------------------------------------------------
void ChoosePlanStepDialog::on_btnCancel_clicked()
{
    this->reject();
}
//----------------------------------------------------------------------------------------------
ChoosePlanStepDialog::~ChoosePlanStepDialog()
{
    Toolbox::MemoryClean(m_autoComplete);
}
