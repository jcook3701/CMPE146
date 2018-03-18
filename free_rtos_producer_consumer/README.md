# Lab 6: Free RTOS Producer Consumer

Question 1) Use the same priority for both tasks, and note down the order of the print-outs
- Sender prints first and receiver prints second on every occurrence.  I assume that this is because of the one second delay that was placed inside of the sender which allows the receiver an appropriate amount of to run without interfering with the sender. 

Question 2) Use higher priority for the receiving task (consumer task), and note down the order of the print-outs.
- Sender prints first and receiver prints second on every occurrence which is the same as when both the consumer and the producer have the same priority. 

Question 3) What if you use ZERO block time while sending an item to the queue, will that make any difference?
- When using Zero for the time block the receiver collects values before the sender has a chance to send anything and continues to attempt to collect values constantly between the senders 100ms delays.  This the result is a page full of read information with very few writes in-between.   

Question 4) What is the purpose of the block time during xQueueReceive()?
- The purpose of the time block is to allow the sender time to add items onto the queue before the receiver attempts to read.  

