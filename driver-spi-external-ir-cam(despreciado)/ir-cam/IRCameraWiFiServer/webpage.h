static const char canvas_htm[] PROGMEM = "<!-- Author : Mudassar Tamboli -->\n"\
"<!DOCTYPE html>\n"\
"<html lang='en'>\n"\
   "<head>\n"\
      "<meta charset='UTF-8'>\n"\
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"\
      "<title>Document</title>\n"\
      "<style>*{margin:0;padding:0}.flipButton{width:470px;height:50px}#temperature{padding:5px;width:470px;font-size:1.8em;text-align:center}</style>\n"\
      "<script>\n"\
            "let startX = 0;\n"\
            "let moveX = 10;\n"\
            "const positive = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'];\n"\
            "const negative = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'];\n"\

            "function decompress(compressed, rowLength) {\n"\
                "const parts = compressed.split('.');\n"\
                "let output = [];\n"\
                "let result = parts[1];\n"\
                "let decimals = parseInt(parts[0].substring(0, 1));\n"\
                "let accuracy = parseInt(parts[0].substring(1, 2));\n"\
                "let start = parseInt(parts[0].substring(2, parts[0].length));\n"\
                "start = start / Math.pow(10.0, decimals);\n"\
                "output.push(start);\n"\
                "let multiplier = '';\n"\
                "let counter = 1;\n"\
                "for (let n = 0; n < result.length; n += 1) {\n"\
                    "let found = false;\n"\
                    "let indexOfItem = positive.indexOf(result[n]);\n"\
                    "if (indexOfItem === -1) {\n"\
                        "indexOfItem = negative.indexOf(result[n]);\n"\
                        "if (indexOfItem >= 0) {\n"\
                           "found = true;\n"\
                            "indexOfItem = -indexOfItem;\n"\
                        "}\n"\
                    "} else {\n"\
                        "found = true;\n"\
                    "}\n"\
                    "if (!found) {\n"\
                        "multiplier += result[n];\n"\
                    "} else {\n"\
                        "if (Math.abs(indexOfItem) > 0) {\n"\
                            "indexOfItem = indexOfItem * accuracy;\n"\
                        "}\n"\
                        "indexOfItem = indexOfItem / Math.pow(10, decimals);\n"\
                        "multiplier = parseInt(multiplier);\n"\
                        "if (counter > 1 && counter % rowLength === 0) {\n"\
                            "start = output[counter - rowLength];\n"\
                        "}\n"\
                        "counter += 1;\n"\
                        "start += indexOfItem;\n"\
                        "output.push(parseFloat(parseFloat(start).toFixed(decimals)));\n"\
                        "while (multiplier > 1) {\n"\
                            "if (counter > 1 && counter % rowLength === 0) {\n"\
                                "start = output[counter - rowLength];\n"\
                            "}\n"\
                            "counter += 1;\n"\
                            "start += indexOfItem;\n"\
                            "output.push(parseFloat(parseFloat(start).toFixed(decimals)));\n"\
                            "multiplier -= 1;\n"\
                        "}\n"\
                        "multiplier = '';\n"\
                    "}\n"\
                "}\n"\
                "return output;\n"\
            "}\n"\

            "function heatMapColorforValue(value, avgLowTemp, avgHighTemp) {\n"\
                "let h;\n"\
                "let l = 50;\n"\
                "let s;\n"\
                "let a = 1;\n"\
                "let threshold = 38;\n"\
                "let diff = avgHighTemp - avgLowTemp;\n"\
                "if (diff < 4) {\n"\
                    "if (avgHighTemp < 75) {\n"\
                        "threshold = 100 - (diff * 4);\n"\
                    "} else if (avgHighTemp > 75) {\n"\
                        "threshold = (diff * 4);\n"\
                    "}\n"\
                "}\n"\
                "if (value < threshold) {\n"\
                    "h = 200 + (1.0 - (value / threshold)) * 30;\n"\
                    "l = 50 + (value / threshold) * 30;\n"\
                    "s = 60 + (value / threshold) * 35;\n"\
                    "a = 0.8;\n"\
                "} else {\n"\
                    "h = 10 + 40 * (1.0 - ((value - threshold) / threshold));\n"\
                    "l = 40 + 50 * (1.0 - ((value - threshold) / threshold));\n"\
                    "s = 98;\n"\
                    "a = 0.95;\n"\
                "}\n"\
                "return `hsla(${h},${s}%,${l}%,${a})`;\n"\
            "}\n"\

            "function percentToRGB(percent) {\n"\
                "if (percent === 100) {\n"\
                    "percent = 99\n"\
                "}\n"\
                "var r, g, b;\n"\
                "var result = '';\n"\
                "if (percent < 50) {\n"\
                    "r = Math.floor(255 * ((percent - 30) / 20));\n"\
                    "g = Math.floor(180 * ((percent) / 50));\n"\
                    "b = Math.floor(255 * ((75 - percent) / 50));\n"\
                    "result = 'rgba(' + r + ',' + g + ',' + b + ', 0.9)';\n"\
                "} else {\n"\
                    "r = 255;\n"\
                    "g = Math.floor(200 * ((50 - percent % 50) / 50));\n"\
                    "b = Math.floor(100 * ((50 - percent % 50) / 50));\n"\
                    "result = 'rgba(' + r + ',' + g + ',' + b + ', 0.9)';\n"\
                "}\n"\
                "return result;\n"\
            "}\n"\

            "function interpolateData(input, rowLength) {\n"\
                "const output = [];\n"\
                "let rowNum = 1;\n"\
                "for (let n = 0; n < input.length; n += 1) {\n"\
                    "if (n % rowLength === 0) {\n"\
                        "output.push((input[n] + input[n + 1]) / 2)\n"\
                    "} else if (n > 0) {\n"\
                        "output.push((input[n] + input[n - 1]) / 2)\n"\
                    "} else {\n"\
                        "output.push(input[n]);\n"\
                    "}\n"\
                    "if ((n + 1) % rowLength === 0) {\n"\
                        "if (input.length - n > rowLength) {\n"\
                            "for (let r = 1; r <= rowLength * 2 - 1; r++) {\n"\
                                "const roundedA = ((rowNum - 1) * rowLength) + Math.floor((r - 1) / 2);\n"\
                                "const roundedB = ((rowNum - 1) * rowLength) + Math.floor((r - 1) / 2);\n"\
                                "const topA = input[roundedA];\n"\
                                "const topB = input[roundedB];\n"\
                                "const roundedC = (rowNum * rowLength) + Math.floor((r - 1) / 2);\n"\
                                "const roundedD = (rowNum * rowLength) + Math.floor((r - 1) / 2);\n"\
                                "const bottomA = input[roundedC];\n"\
                                "const bottomB = input[roundedD];\n"\
                                "output.push((topA + topB + bottomA + bottomB) / 4);\n"\
                            "}\n"\
                            "rowNum += 1;\n"\
                        "}\n"\
                    "} else {\n"\
                        "output.push((input[n] + input[n + 1]) / 2)\n"\
                    "}\n"\
                "}\n"\
                "return output;\n"\
            "}\n"\
            "let url = 'ws://thermal.local:81';\n"\
            "if (typeof ipAddress !== 'undefined') {\n"\
                "url = 'ws://' + ipAddress + ':81';\n"\
            "}\n"\
            "let connection = new WebSocket(url);\n"\
            "connection.onopen = function() {\n"\
                "connection.send('Ping');\n"\
            "};\n"\
            "connection.onerror = function(error) {\n"\
                "console.log('WebSocket Error ' + error);\n"\
            "};\n"\
            "let avgHighTemp = 0;\n"\
            "let avgLowTemp = 0;\n"\
            "let highTarget = -100;\n"\
            "let lowTarget = 200;\n"\
            "let high = -100;\n"\
            "let low = 200;\n"\
            "let counter = 30;\n"\
            "connection.onmessage = function(e) {\n"\
                "if (e.data.length < 100) {\n"\
                    "return;\n"\
                "}\n"\
                "const values = decompress(e.data, 32);\n"\
                "const tempValues = interpolateData(values, 32);\n"\
                "const sortedValues = values.slice().sort();\n"\
                "avgHighTemp = 0;\n"\
                "for (let w = sortedValues.length - 3; w > sortedValues.length - 10; w -= 1) {\n"\
                    "avgHighTemp += sortedValues[w];\n"\
                "}\n"\
                "avgHighTemp = avgHighTemp / 7;\n"\
                "avgLowTemp = 0;\n"\
                "for (let w = 4; w < 24; w += 1) {\n"\
                    "avgLowTemp += sortedValues[w];\n"\
                "}\n"\
                "avgLowTemp = avgLowTemp / 20;\n"\
                "if (counter >= 20) {\n"\
                    "let avgHighTempF = avgHighTemp * 1.8 + 32;\n"\
                    "avgHighTempF = parseInt(avgHighTempF * 10) / 10;\n"\
                    "let avgLowTempF = avgLowTemp * 1.8 + 32;\n"\
                    "avgLowTempF = parseInt(avgLowTempF * 10) / 10;\n"\
                    "document.getElementById('temperature').innerText = 'High: ' + avgHighTempF + ' F | Low: ' + avgLowTempF + ' F';\n"\
                    "highTarget = -100;\n"\
                    "lowTarget = 200;\n"\
                    "for (let i = 0; i < values.length; i += 1) {\n"\
                        "if (values[i] < 120 && values[i] > highTarget) {\n"\
                            "highTarget = parseInt(values[i]);\n"\
                        "}\n"\
                        "if (values[i] < lowTarget) {\n"\
                            "lowTarget = parseInt(values[i]);\n"\
                        "}\n"\
                    "}\n"\
                    "if (highTarget - lowTarget < 10) {\n"\
                        "highTarget += 5;\n"\
                    "}\n"\
                    "lowTarget += 1;\n"\
                    "counter = 0;\n"\
                    "return;\n"\
                "}\n"\
                "counter += 1;\n"\
                "if (Math.abs(low - avgLowTemp) < 1.5) {} else if (low === 200) {\n"\
                    "low = avgLowTemp;\n"\
                "} else if (low > avgLowTemp) {\n"\
                    "low -= 1;\n"\
                "} else if (low < avgLowTemp) {\n"\
                    "low += 1;\n"\
                "}\n"\
                "if (Math.abs(high - avgHighTemp) < 1.5) {} else if (high === -100) {\n"\
                    "high = avgHighTemp;\n"\
                "} else if (high > avgHighTemp) {\n"\
                    "high -= 1;\n"\
                "} else if (high < avgHighTemp) {\n"\
                    "high += 1;\n"\
                "}\n"\
                "var c2 = document.getElementById('myCanvas');\n"\
                "var c2_context = c2.getContext('2d');\n"\
                "let xPos = startX;\n"\
                "let yPos = 0;\n"\
                "for (let i = 0; i < tempValues.length; i += 1) {\n"\
                    "let value = ((tempValues[i] - low) / (high - low)) * 100;\n"\
                    "c2_context.fillStyle = heatMapColorforValue(value, low, high);\n"\
                    "c2_context.fillRect(xPos, yPos, 10, 10);\n"\
                    "yPos += 10;\n"\
                    "if (i > 0 && (i + 1) % 63 === 0) {\n"\
                        "xPos += moveX;\n"\
                        "yPos = 0;\n"\
                    "}\n"\
                "}\n"\
            "};\n"\

            "function flip() {\n"\
                "if (startX === 0) {\n"\
                    "startX = 460;\n"\
                    "moveX = -10;\n"\
                "} else {\n"\
                    "startX = 0;\n"\
                    "moveX = 10;\n"\
                "}\n"\
            "}\n"\
      "</script> \n"\
   "</head>\n"\

   "<body>\n"\
      "<canvas id='myCanvas' width='470' height='630' style='border:1px solid #d3d3d3;'></canvas>\n"\
      "<p id='temperature'></p>\n"\
      "<button class='flipButton' onclick='flip()'>Flip Camera</button>\n"\
      "<p id='ip'></p>\n"\
   "</body>\n"\

   "<script>\n"\
        "if (typeof ipAddress !== 'undefined') {\n"\
            "document.getElementById('ip').innerText = 'IP: ' + ipAddress;\n"\
        "}\n"\
    "</script> \n"\

"</html>\n"\
;
