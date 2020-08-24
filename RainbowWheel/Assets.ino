// All assets are saved here
String logo = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?> <svg    xmlns:dc=\"http://purl.org/dc/elements/1.1/\"    xmlns:cc=\"http://creativecommons.org/ns#\"    xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"    xmlns:svg=\"http://www.w3.org/2000/svg\"    xmlns=\"http://www.w3.org/2000/svg\"    xmlns:xlink=\"http://www.w3.org/1999/xlink\"    xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"    xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"    width=\"382\"    height=\"382\"    viewBox=\"-5 -5 10 10\"    version=\"1.1\"    id=\"svg29\"    sodipodi:docname=\"BYR_color_wheel.svg\"    inkscape:version=\"0.92.4 (5da689c313, 2019-01-14)\">   <metadata      id=\"metadata33\">     <rdf:RDF>       <cc:Work          rdf:about=\"\">         <dc:format>image/svg+xml</dc:format>         <dc:type            rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />       </cc:Work>     </rdf:RDF>   </metadata>   <sodipodi:namedview      pagecolor=\"#ffffff\"      bordercolor=\"#666666\"      borderopacity=\"1\"      objecttolerance=\"10\"      gridtolerance=\"10\"      guidetolerance=\"10\"      inkscape:pageopacity=\"0\"      inkscape:pageshadow=\"2\"      inkscape:window-width=\"1920\"      inkscape:window-height=\"1001\"      id=\"namedview31\"      showgrid=\"false\"      showguides=\"true\"      inkscape:guide-bbox=\"true\"      inkscape:zoom=\"2.4712042\"      inkscape:cx=\"70.554525\"      inkscape:cy=\"194.58493\"      inkscape:window-x=\"0\"      inkscape:window-y=\"0\"      inkscape:window-maximized=\"1\"      inkscape:current-layer=\"svg29\">     <sodipodi:guide        position=\"3.845339,5\"        orientation=\"0,1\"        id=\"guide845\"        inkscape:locked=\"false\" />     <sodipodi:guide        position=\"4.9894068,9.4756356\"        orientation=\"1,0\"        id=\"guide847\"        inkscape:locked=\"false\" />   </sodipodi:namedview>   <defs      id=\"defs3\">     <path        id=\"a\"        d=\"M2.991,0.815A3.1,3.1 0 0 0 2.991,-0.815L4.825,-1.311A5,5 0 0 1 4.825,1.311\" />   </defs>   <use      xlink:href=\"#a\"      fill=\"#fd5308\"      id=\"use5\" />   <use      xlink:href=\"#a\"      fill=\"#fe2712\"      transform=\"rotate(30)\"      id=\"use7\" />   <use      xlink:href=\"#a\"      fill=\"#a7194b\"      transform=\"rotate(60)\"      id=\"use9\" />   <use      xlink:href=\"#a\"      fill=\"#8601af\"      transform=\"rotate(90)\"      id=\"use11\" />   <use      xlink:href=\"#a\"      fill=\"#3d01a4\"      transform=\"rotate(120)\"      id=\"use13\" />   <use      xlink:href=\"#a\"      fill=\"#0247fe\"      transform=\"rotate(150)\"      id=\"use15\" />   <use      xlink:href=\"#a\"      fill=\"#0392ce\"      transform=\"rotate(180)\"      id=\"use17\" />   <use      xlink:href=\"#a\"      fill=\"#66b032\"      transform=\"rotate(210)\"      id=\"use19\" />   <use      xlink:href=\"#a\"      fill=\"#d0ea2b\"      transform=\"rotate(240)\"      id=\"use21\" />   <use      xlink:href=\"#a\"      fill=\"#fefe33\"      transform=\"rotate(270)\"      id=\"use23\" />   <use      xlink:href=\"#a\"      fill=\"#fabc02\"      transform=\"rotate(300)\"      id=\"use25\" />   <use      xlink:href=\"#a\"      transform=\"rotate(330)\"      id=\"use27\"      fill=\"#fb9902\" />   <circle      style=\"fill:none;fill-opacity:1;stroke:#000000;stroke-width:0.26178011;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:4;stroke-dasharray:none;stroke-dashoffset:0;stroke-opacity:1\"      id=\"path843\"      cx=\"-0.016001539\"      cy=\"-0.0054970882\"      r=\"0.40249759\" /> </svg> ";

String manifest = "{   \"name\": \"Rainbow Controller\",   \"short_name\": \"Rainbow\",   \"start_url\": \"index.html\",   \"display\": \"standalone\",   \"background_color\": \"#fff\",   \"description\": \"Control your rainbow wheel\",   \"orientation\": \"portrait-primary\",   \"icons\": [     {       \"src\": \"./assets/logo.svg\",       \"sizes\": \"513x513\",       \"type\": \"image/svg\"     }   ] }  ";

String rainbowcontroller = "<!DOCTYPE html> <html lang=\"en\"> <head>     <meta charset=\"UTF-8\">     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">     <link rel=\"icon\" type=\"image/svg\" href=\"./logo.svge\"/>     <link rel=\"manifest\" href=\"./manifest.webmanifest\">     <title>Rainbow Wheel Controller</title>     <style>        .slidecontainer {         width: 100%;     }      .slider {         -webkit-appearance: none;         appearance: none;         width: 100%;         height: 25px;         background: #d3d3d3;         outline: none;         opacity: 0.7;         -webkit-transition: .2s;         transition: opacity .2s;     }      .slider:hover {         opacity: 1;     }      .slider::-webkit-slider-thumb {         -webkit-appearance: none;         appearance: none;         width: 25px;         height: 25px;         background: #ffef16;         cursor: pointer;     }      #red.slider::-webkit-slider-thumb {         background: #cc0000;     }      #green.slider::-webkit-slider-thumb {         background: #00aa00;     }      #blue.slider::-webkit-slider-thumb {         background: #4a71f6     }      .slider::-moz-range-thumb {         width: 25px;         height: 25px;         cursor: pointer;         background: #ffef16;     }      #red.slider::-moz-range-thumb {         background: #cc0000;     }      #green.slider::-moz-range-thumb {         background: #00aa00;     }      #blue.slider::-moz-range-thumb {         background: #4a71f6;     }    </style> </head> <body> <div class=\"slidecontainer\"><h3>Colour intensity</h3>     <input type=\"range\" min=\"0\" max=\"200\" value=\"50\" class=\"slider\" id=\"red\">     <input type=\"range\" min=\"0\" max=\"200\" value=\"50\" class=\"slider\" id=\"green\">     <input type=\"range\" min=\"0\" max=\"200\" value=\"50\" class=\"slider\" id=\"blue\">     <h3>Speed</h3>     <input type=\"range\" min=\"1\" max=\"60\" value=\"10\" class=\"slider\" id=\"speed\">     <h3>Width</h3>     <input type=\"range\" min=\"1\" max=\"104\" value=\"10\" class=\"slider\" id=\"width\">     <label for=\"modepicker\">Mode</label>     <select name=\"modepicker\" id=\"modepicker\">         <option value=\"0\">Rotating rainbow/fixed colour</option>         <option value=\"1\">Clock</option>         <option value=\"2\">Moving lights</option>         <option value=\"3\">Sectors</option>     </select>      <script>          let red = document.getElementById(\"red\");         let green = document.getElementById(\"green\");         let blue = document.getElementById(\"blue\");         let speed = document.getElementById(\"speed\");         let width = document.getElementById(\"width\");         let modeElement = document.getElementById(\"modepicker\");         let updatePending = true; /* Perform an update on load to set the clock*/                  function configure() {             if (!updatePending) {                 return;             }             updatePending = false;                          let now = new Date();             let today = new Date();             today.setHours(0, 0, 0, 0);             /* Update the time while we are at it*/             let millisSinceEpoch = now - today;                               var xhr = new XMLHttpRequest();             xhr.open(\"GET\", `./configure?millisSinceToday=${millisSinceEpoch}&mode=${modeElement.value}&r=${red.value}&g=${green.value}&b=${blue.value}&w=${width.value}&s=${61 - speed.value}`);             xhr.send();         };          function chronic() {             configure();             window.setTimeout(chronic, 400);         };         chronic();          function update() {             updatePending = true;         };          modeElement.oninput = update;         red.oninput = update;         blue.oninput = update;         green.oninput = update;         speed.oninput = update;         width.oninput = update;     </script> </div> </body> </html>  ";

