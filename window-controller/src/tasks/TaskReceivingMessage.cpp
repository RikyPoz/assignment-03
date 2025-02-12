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
        String msg = MsgService.receiveMsg()->getContent();
        //Serial.println("[ARDUINO] ricevo MSG: "+msg);
        if (msg.startsWith("temperature_"))
        {
            float temp = msg.substring(12).toFloat();
            //Serial.println("[ARDUINO] ricevo temp: "+String(temp));
            if (window->getTemp() != temp)
            {
                window->updateTemp(temp);
            }
        }
        else if (msg.startsWith("position_"))
        {
            int value = msg.substring(9).toInt();
            //Serial.println("[ARDUINO] ricevo pos: "+String(value));
            if (window->getDashboardValue() != value)
            {
                window->updateDashboardValue(value);
            }
        }
        else if (msg.startsWith("mode_"))
        {
            String mode = msg.substring(5);
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
            Serial.println("Error: Unknown message: "+msg);
        }
    }
}
