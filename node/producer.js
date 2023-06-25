const { Kafka } = require('kafkajs');

const kafka = new Kafka({
    clientId: 'my-kafka-app',
    brokers: ['localhost:9092'], // Replace with your broker information
  });

const producer = kafka.producer();

const sendMessage = async () => {
    try {
      await producer.connect();
      await producer.send({
        topic: 'my-topic', // Replace with your desired topic
        messages: [
          { value: 'Hello Kafka!' },
        ],
      });
      console.log('Message sent successfully!');
    } catch (error) {
      console.error('Error sending message:', error);
    } finally {
      await producer.disconnect();
    }
};

sendMessage();
