document.addEventListener('DOMContentLoaded', () => {
    const heading = document.querySelector('h1');

    if (heading) {
        heading.addEventListener('click', () => {
            const randomColor = '#' + Math.floor(Math.random()*16777215).toString(16).padStart(6, '0');
            heading.style.backgroundColor = randomColor;
        });
    } else {
        console.error('Heading element not found!');
    }

    // --- Chat Functionality ---
    const messagesContainer = document.getElementById('chat-messages');
    const messageInput = document.getElementById('chat-message-input') as HTMLInputElement | null;
    const sendButton = document.getElementById('chat-send-button');

    if (!messagesContainer || !messageInput || !sendButton) {
        console.error('Chat elements not found!');
        return;
    }

    // Function to display messages
    const displayMessages = (messages: { timestamp: string, text: string }[]) => {
        messagesContainer.innerHTML = ''; // Clear existing messages
        if (messages.length === 0) {
            messagesContainer.innerHTML = '<p>No messages yet.</p>';
            return;
        }
        messages.forEach(msg => {
            const messageElement = document.createElement('div');
            messageElement.classList.add('chat-message');
            // Basic escaping to prevent HTML injection
            const safeText = msg.text.replace(/</g, "&lt;").replace(/>/g, "&gt;");
            messageElement.innerHTML = `<span class="timestamp">[${msg.timestamp}]</span>: <span class="text">${safeText}</span>`;
            messagesContainer.appendChild(messageElement);
        });
        // Scroll to the bottom
        messagesContainer.scrollTop = messagesContainer.scrollHeight;
    };

    // Function to fetch messages
    const fetchMessages = async () => {
        try {
            const response = await fetch('/api/chat/messages');
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const messages = await response.json();
            displayMessages(messages);
        } catch (error) {
            console.error('Error fetching messages:', error);
            messagesContainer.innerHTML = '<p>Error loading messages.</p>';
        }
    };

    // Function to send a message
    const sendMessage = async () => {
        const messageText = messageInput.value.trim();
        if (messageText === '') {
            return; // Don't send empty messages
        }

        try {
            const response = await fetch('/api/chat/messages', {
                method: 'POST',
                headers: {
                    'Content-Type': 'text/plain', // Sending plain text body
                },
                body: messageText,
            });

            if (!response.ok) {
                 throw new Error(`HTTP error! status: ${response.status}`);
            }

            // Clear input and refresh messages
            messageInput.value = '';
            await fetchMessages();

        } catch (error) {
            console.error('Error sending message:', error);
            alert('Failed to send message.'); // Simple user feedback
        }
    };

    // Event listeners
    sendButton.addEventListener('click', sendMessage);
    messageInput.addEventListener('keypress', (event) => {
        if (event.key === 'Enter') {
            sendMessage();
        }
    });

    // Initial fetch of messages
    fetchMessages();

    // Optional: Poll for new messages periodically (simple approach)
    // setInterval(fetchMessages, 5000); // Refresh every 5 seconds

});

