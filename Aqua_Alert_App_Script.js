function doPost(e) {
  if (typeof e !== 'undefined' && e.postData.contents) {
    var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
    var json = JSON.parse(e.postData.contents);
    var timestamp = new Date();
    var sensorValue = json.value1;

    sheet.appendRow([timestamp, sensorValue]);

    if (sensorValue > 300) { // 閾値を超えた場合に通知を送る（適宜変更）
      sendNotification(sensorValue);
    }

    return ContentService.createTextOutput("Success");
  } else {
    return ContentService.createTextOutput("No POST data received");
  }
}

function sendNotification(sensorValue) {
  var emailAddress = "your_email@example.com"; // 通知を受け取るメールアドレス
  var subject = "Water Sensor Alert";
  var message = "Water sensor value: " + sensorValue;
  MailApp.sendEmail(emailAddress, subject, message);
}
