document.addEventListener("DOMContentLoaded", () => {
  const heading = document.querySelector("h1");

  if (heading) {
    heading.addEventListener("click", () => {
      const randomColor =
        "#" +
        Math.floor(Math.random() * 16777215)
          .toString(16)
          .padStart(6, "0");
      heading.style.backgroundColor = randomColor;
    });
  } else {
    console.error("Heading element not found!");
  }

  const messagesContainer = document.getElementById("chat-messages");
  const messageInput = document.getElementById(
    "chat-message-input"
  ) as HTMLInputElement | null;
  const sendButton = document.getElementById("chat-send-button");

  if (!messagesContainer || !messageInput || !sendButton) {
    console.error("Chat elements not found!");
    return;
  }

  const displayMessages = (messages: { timestamp: string; text: string }[]) => {
    messagesContainer.innerHTML = "";
    if (messages.length === 0) {
      messagesContainer.innerHTML = "<p>No messages yet.</p>";
      return;
    }
    messages.forEach((msg) => {
      const messageElement = document.createElement("div");
      messageElement.classList.add("chat-message");

      const safeText = msg.text.replace(/</g, "&lt;").replace(/>/g, "&gt;");
      messageElement.innerHTML = `<span class="timestamp">[${msg.timestamp}]</span>: <span class="text">${safeText}</span>`;
      messagesContainer.appendChild(messageElement);
    });

    messagesContainer.scrollTop = messagesContainer.scrollHeight;
  };

  const fetchMessages = async () => {
    try {
      const response = await fetch("/api/chat/messages");
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const messages = await response.json();
      displayMessages(messages);
    } catch (error) {
      console.error("Error fetching messages:", error);
      messagesContainer.innerHTML = "<p>Error loading messages.</p>";
    }
  };

  const sendMessage = async () => {
    const messageText = messageInput.value.trim();
    if (messageText === "") {
      return;
    }

    try {
      const response = await fetch("/api/chat/messages", {
        method: "POST",
        headers: {
          "Content-Type": "text/plain",
        },
        body: messageText,
      });

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }

      messageInput.value = "";
      await fetchMessages();
    } catch (error) {
      console.error("Error sending message:", error);
      alert("Failed to send message.");
    }
  };

  sendButton.addEventListener("click", sendMessage);
  messageInput.addEventListener("keypress", (event) => {
    if (event.key === "Enter") {
      sendMessage();
    }
  });

  fetchMessages();
});
