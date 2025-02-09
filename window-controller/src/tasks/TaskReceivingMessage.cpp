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
        Serial.println("ricevo");
        String msg = MsgService.receiveMsg()->getContent();
        // se facciamo 2 msg separati
        if (msg.startsWith("temperature_"))
        {
            String temp = msg.substring(11);
            window->updateTemp(temp);
        }
        else if (msg.startsWith("position_"))
        {
            int level = msg.substring(8).toInt();
            window->updateWindowLevel(level);
        }
        else if (msg.startsWith("mode_"))
        {
            String mode = msg.substring(4);
            if (mode.equals("AUTOMATIC")) {
                window->notifyAutomatic();
            } else if (mode.equals("MANUAL")) {
                window->notifyManual();
            }
        }
        else
        {
            Serial.println("Error: Unknown message");
        }
    }
}
