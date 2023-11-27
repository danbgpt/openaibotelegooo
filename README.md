# openaibotelegooo

This code creates an http server locally where the esp32 wrover sends a picture every second. This picture is sent to the gpt4 image analyzer api with the prompt to return direction commands. These direction commands are then sent to endpoints on the esp32 wrover module waiting for those commands. The commands once recieved in esp32, are sent in serial to the arduino uno board. The board then triggers the proper movement commands. 
