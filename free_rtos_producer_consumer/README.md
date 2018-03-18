#Lab 6: free_rtos_producer_consumer

Use the same priority for both tasks, and note down the order of the print-outs
- Sender prints first and reciever prints second on every occurance.  I assume that this is becuase of the one second delay that was placed inside of the sender which allows the reciever an apropriate amount of to run without interfering with the sender. 

Use higher priority for the receiving task (consumer task), and note down the order of the print-outs.
- Sender prints first and reciever prints second on every occurance which is the same as when both the consumer and the producer have the same priority. 

What if you use ZERO block time while sending an item to the queue, will that make any difference?
- When using Zero for the time block the reciever collects values before the sender has a chance to send anything and continues to attempt to collect values constantly between the senders 100ms delays.  This the result is a page full of read infomation with very few writes inbwetween.   

What is the purpose of the block time during xQueueReceive() ?
- The purpose of the time block is to allow the sender time to add items onto the que before the receiver attempts to read.  

