#include "TaskReceivingMessage.h"
#include "kernel/MsgService.h"

TaskReceivingMessage::TaskReceivingMessage(Window *window)
{
    this->window = window;
}

void TaskReceivingMessage::tick()
{
    if (MsgService.isMsgAvailable())
    {
        Msg* message = MsgService.receiveMsg();
        String msg = message->getContent();
        if (msg.startsWith("temperature_"))
        {
            float temp = msg.substring(12).toFloat();
            if (window->getTemp() != temp)
            {
                window->updateTemp(temp);
            }
        }
        else if (msg.startsWith("position_"))
        {
            int value = msg.substring(9).toInt();
            if (window->getDashboardValue() != value)
            {
                window->updateDashboardValue(value);
            }
        }
        else if (msg.startsWith("mode_"))
        {
            String mode = msg.substring(5);
            if (mode.equals("AUTOMATIC") && window->isManual())
            {
                window->notifyAutomatic();
            }
            else if (mode.equals("MANUAL") && window->isAuto())
            {
                window->notifyManual();
            }
        }
        else if (msg.startsWith("alarm_"))
        {
            if (msg.substring(6).toInt()) {
                window->notifyAlarm();
            } else {
                window->notifyAlarmResetted();
            }
        }
        else
        {
            Serial.println("Error: Unknown message: "+msg);
        }
        delete message;
    }
}
