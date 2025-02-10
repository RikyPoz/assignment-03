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
            String temp = msg.substring(11);
            window->updateTemp(temp);
        }
        else if (msg.startsWith("position_"))
        {
            int value = msg.substring(8).toInt();
            window->updateDashboardValue(value);
        }
        else if (msg.startsWith("mode_"))
        {
            String mode = msg.substring(4);
            if (mode.equals("AUTOMATIC"))
            {
                window->notifyAutomatic();
            }
            else if (mode.equals("MANUAL"))
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
