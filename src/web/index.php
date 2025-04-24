<?php header('Content-Type: text/html; charset=utf-8'); ?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home & Chat</title>
    <link rel="stylesheet" href="/assets/css/main.css">
</head>
<body>
    <div id="myDiv">
      <h1>
          <p>Hello!</p>
      </h1>
    </div>

    <div class="chat-container">
        <h2>Chat</h2>
        <div id="chat-messages" class="chat-messages">
        </div>
        <div class="chat-input">
            <input type="text" id="chat-message-input" placeholder="Enter your message...">
            <button id="chat-send-button">Send</button>
        </div>
    </div>

    <script src="/assets/js/main.js"></script>
</body>
<footer>
    <div id="myDiv">
        <h1>
            <ps>👍🏼</ps>
        </h1>
    </div>
</footer>
</html>
