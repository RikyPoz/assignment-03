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
        Serial.println("Messaggio ricevuto dalla central unit");
        String msg = MsgService.receiveMsg()->getContent();
        if (msg.startsWith("temperature_"))
        {
            float temp = msg.substring(11).toFloat();
            if (window->getTemp() != temp)
            {
                window->updateTemp(temp);
            }
        }
        else if (msg.startsWith("position_"))
        {
            int value = msg.substring(8).toInt();
            if (window->getDashboardValue() != value)
            {
                window->updateDashboardValue(value);
            }
        }
        else if (msg.startsWith("mode_"))
        {
            String mode = msg.substring(4);
            if (mode.equals("AUTOMATIC") && !window->isAuto())
            {
                window->notifyAutomatic();
            }
            else if (mode.equals("MANUAL") && window->isAuto())
            {
                window->notifyManual();
            }
        }
        else
        {
            Serial.println("Error: Unknown message");
        }
    }
}
