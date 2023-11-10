#ifndef _WEB_STRINGS_H_
#define _WEB_STRINGS_H_
static const char index_html[] PROGMEM = "<html lang='en'>"
  "<head>"
    "<meta name='format-detection' content='telephone=no'>"
    "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'>"
    "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>"
    "<script src='https://code.jquery.com/jquery-3.6.0.min.js' integrity='sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4='crossorigin='anonymous'></script>"
    "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js' integrity='sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q' crossorigin='anonymous'></script>"
    "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js' integrity='sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl' crossorigin='anonymous'></script>"
    "<script src='https://cdn.jsdelivr.net/npm/select2@4.1.0-rc.0/dist/js/select2.min.js'></script>"
    "<link rel='preconnect' href='https://fonts.googleapis.com'><link rel='preconnect' href='https://fonts.gstatic.com' crossorigin=''>"
    "<link href='https://cdn.jsdelivr.net/npm/select2@4.1.0-rc.0/dist/css/select2.min.css' rel='stylesheet' />"
    "<link href='https://fonts.googleapis.com/css2?family=Overpass:wght@900&amp;display=swap' rel='stylesheet'>"
    "<title>L1Dcube Settings</title>"

    "<script>"
      "$(document).ready(function () {"
        "$('#submitAlert').hide();"
        "$.getJSON( 'https://api.exchange.coinbase.com/products', function( data ) {"
          "var items = [];"
          "$.each( data, function( key, val ) {"
            "items.push( { id : val.id , text: val.id } );"
          "});"
          "items.sort((a, b) => {"
              "let fa = a.id.toLowerCase(),"
                  "fb = b.id.toLowerCase();"

              "if (fa < fb) {"
                  "return -1;"
              "}"
              "if (fa > fb) {"
                  "return 1;"
              "}"
              "return 0;"
          "});"
          "$('#currencyPairs').select2({"
              "width: 'resolve', /* need to override the changed default*/"
              "data: items,"
              "maximumSelectionLength: 5"
          "});"
          "$('#currencyPairs').val([%CURRENCY_PAIRS%]);"
          "$('#currencyPairs').trigger('change');"
        "});"
        "$('#settingsForm').submit(function (event) {"
            "$('#submitAlert').show();"
            "setTimeout(location.reload.bind(location), 5000);"
        "});"
      "});"
    "</script>"
  "</head>"
  "<body>"
    "<header class='flex-column flex-md-row navbar' style='background-color: #4d4d4e;color: #ffffff;'>"
      "<span style=\"font-family: 'Overpass', sans-serif;\">L1Dcube Settings</span>"
    "</header>"
    "<div class='container'>"
      "<h4>Price change thresholds<a class='font-weight-bold' data-toggle='collapse' href='#collapseThresholds' aria-expanded='false' aria-controls='collapseThresholds'> ?</a></h4>"
      "<div class='collapse' id='collapseThresholds'>"
        "<div class='mb-2 alert alert-info'>Thresholds affect sesitivity of LEDs to price change. Price change means difference between two displayed prices of the same currency pair in a "
          "row in case of ticker or difference between checkpoint value and current price in case of long term watcher. The lower the <a href='#screenChangeDelay'>screen change delay</a>, the smaller the price "
          "difference and the lower the threshold (higher sensitivity) needed to activate LEDs.</div>"
      "</div>"
      "<form action='/get' target='hidden-form' id='settingsForm'>"
        "<div class='mb-2'>"
          "<label for='inputLEDtickThresh' class='col-form-label'>Price ticker (in percent)"
            "<a class='font-weight-bold' data-toggle='collapse' href='#collapseHelpLED' aria-expanded='false' aria-controls='collapseHelpLED'> ?</a>"
          "</label>"
          "<div class='mb-2'>"
            "<input type='number' name='inputLEDtickThresh' step='0.01' min='0' max='100' value='%INPUT_LED_TICK_THRESH%' class='form-control' required>"
          "</div>"
        "</div>"
        "<div class='collapse' id='collapseHelpLED'>"
          "<div class='mb-2 alert alert-info'>This value has to be between 0 (highest price change sensitivity) and 100 (turned off).<hr>"
            "<p class='mb-0'>Example: current price 1&nbsp;000, threshold 0.01; 1&nbsp;000*(0.01/100)=0.1. LED flashes when price decreases (red) or increases (green) by 0.1 to 999.9 and 1000.1 respectively.</p>"
          "</div>"
        "</div>"
        "<div class='mb-2'>"
          "<label for='inputCPThresh' class='col-form-label'>Long term price watcher (in percent)"
            "<a class='font-weight-bold' data-toggle='collapse' href='#collapseHelpCPThresh' aria-expanded='false' aria-controls='collapseHelpCPThresh'> ?</a>"
          "</label>"
          "<div class='mb-2'>"
            "<input type='number' name='inputCPThresh' step='0.1' min='0.1' value='%INPUT_CP_THRESH%' class='form-control' required>"
          "</div>"
        "</div>"
        "<div class='collapse' id='collapseHelpCPThresh'>"
          "<div class='mb-2 alert alert-info'>This value has to be higher or equal to 0.1. When checkpoint value and current price differs by specified number of percent, "
            "LEDs are activated. Initial checkpoint value for each currency pair is equal to first displayed price after device startup. When the threshold is reached, the new "
            "checkpoint is also created. Current <a href='#priceCheckpoints'>price checkpoints</a> are displayed below.<hr>"
            "<p class='mb-0'>Example: checkpoint 50&nbsp;000, threshold 5; 50&nbsp;000*(5/100)=2500. When price 52&nbsp;500 or 47&nbsp;500 is reached, "
            "LEDs flash and a new checkpoint with the current value is also created.</p>"
          "</div>"
        "</div>"
        "<div class='mb-2'>"
          "<div class='mb-2 alert alert-info'>The higher the threshold values, the less often the LEDs activated."
          "</div>"
        "</div>"
        "<h4>Currency pairs</h4>"
        "<div class='form-group'>"
          "<label for='currencyPairs'>Display selection</label>"
          "<select class='form-control' id='currencyPairs' name='inputCurrencyPairs' multiple='multiple' required='required'>"
            "<option value='BTC-USD'>BTC-USD</option>"
            "<option value='BTC-EUR'>BTC-EUR</option>"
            "<option value='ETH-USD'>ETH-USD</option>"
            "<option value='ETH-EUR'>ETH-EUR</option>"
            "<option value='ETH-BTC'>ETH-BTC</option>"
          "</select>"
        "</div>"
        "<h5 id='priceCheckpoints'>Price checkpoints</h5>"
        "<p><small>Only values greater or equal to 0.1 can be edited.</small></p>"
        "%CURRENCY_CHECKPOINTS%"
        "<h4>Screen</h4>"
        "<div class='mb-2' id='screenChangeDelay'>"
          "<label for='inputScreenChangeDelay' class='col-form-label'>Screen change delay</label>"
          "<div class='mb-2'>"
            "<select name='inputScreenChangeDelay' class='form-control'>"
              "<option value='2000' %SCREEN_CHANGE_DELAY2000%>2s</option>"
              "<option value='5000' %SCREEN_CHANGE_DELAY5000%>5s</option>"
              "<option value='10000' %SCREEN_CHANGE_DELAY10000%>10s</option>"
              "<option value='60000' %SCREEN_CHANGE_DELAY60000%>60s</option>"
              "<option value='300000' %SCREEN_CHANGE_DELAY300000%>5m</option>"
            "</select>"
          "</div>"
        "</div>"
        "<div id='submitAlert' class='alert alert-success'>"
          "Settings saved. L1Dcube will restart in few seconds."
        "</div>"
        "<button type='submit' class='btn btn-primary mb-2'>Save</button>"
      "</form>"
      "<iframe style='display:none' name='hidden-form'></iframe>"
    "</div>"
  "</body>"
"</html>";
#endif
