#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 4
#define DHTTYPE    DHT11     
DHT_Unified dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
#define PIN 2
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
Preferences prefs;
uint32_t delayMS;
const uint16_t colors[] = {
  matrix.Color(255, 255, 255),matrix.Color(214, 103, 93), matrix.Color(255, 0, 255) };
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='en'>
  <head>
    <meta charset='UTF-8' />
    <meta name='viewport' content='width=device-width, initial-scale=1.0' />
    <title>EEE371 Project</title>
    <style>
      /*! tailwindcss v3.4.1 | MIT License | https://tailwindcss.com*/*,:after,:before{box-sizing:border-box;border:0 solid #e5e7eb}:after,:before{--tw-content:''}:host,html{line-height:1.5;-webkit-text-size-adjust:100%;-moz-tab-size:4;-o-tab-size:4;tab-size:4;font-family:ui-sans-serif,system-ui,sans-serif,Apple Color Emoji,Segoe UI Emoji,Segoe UI Symbol,Noto Color Emoji;font-feature-settings:normal;font-variation-settings:normal;-webkit-tap-highlight-color:transparent}body{margin:0;line-height:inherit}hr{height:0;color:inherit;border-top-width:1px}abbr:where([title]){-webkit-text-decoration:underline dotted;text-decoration:underline dotted}h1,h2,h3,h4,h5,h6{font-size:inherit;font-weight:inherit}a{color:inherit;text-decoration:inherit}b,strong{font-weight:bolder}code,kbd,pre,samp{font-family:ui-monospace,SFMono-Regular,Menlo,Monaco,Consolas,Liberation Mono,Courier New,monospace;font-feature-settings:normal;font-variation-settings:normal;font-size:1em}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:initial}sub{bottom:-.25em}sup{top:-.5em}table{text-indent:0;border-color:inherit;border-collapse:collapse}button,input,optgroup,select,textarea{font-family:inherit;font-feature-settings:inherit;font-variation-settings:inherit;font-size:100%;font-weight:inherit;line-height:inherit;color:inherit;margin:0;padding:0}button,select{text-transform:none}[type=button],[type=reset],[type=submit],button{-webkit-appearance:button;background-color:initial;background-image:none}:-moz-focusring{outline:auto}:-moz-ui-invalid{box-shadow:none}progress{vertical-align:initial}::-webkit-inner-spin-button,::-webkit-outer-spin-button{height:auto}[type=search]{-webkit-appearance:textfield;outline-offset:-2px}::-webkit-search-decoration{-webkit-appearance:none}::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}summary{display:list-item}blockquote,dd,dl,figure,h1,h2,h3,h4,h5,h6,hr,p,pre{margin:0}fieldset{margin:0}fieldset,legend{padding:0}menu,ol,ul{list-style:none;margin:0;padding:0}dialog{padding:0}textarea{resize:vertical}input::-moz-placeholder,textarea::-moz-placeholder{opacity:1;color:#9ca3af}input::placeholder,textarea::placeholder{opacity:1;color:#9ca3af}[role=button],button{cursor:pointer}:disabled{cursor:default}audio,canvas,embed,iframe,img,object,svg,video{display:block;vertical-align:middle}img,video{max-width:100%;height:auto}[hidden]{display:none}*,::backdrop,:after,:before{--tw-border-spacing-x:0;--tw-border-spacing-y:0;--tw-translate-x:0;--tw-translate-y:0;--tw-rotate:0;--tw-skew-x:0;--tw-skew-y:0;--tw-scale-x:1;--tw-scale-y:1;--tw-pan-x: ;--tw-pan-y: ;--tw-pinch-zoom: ;--tw-scroll-snap-strictness:proximity;--tw-gradient-from-position: ;--tw-gradient-via-position: ;--tw-gradient-to-position: ;--tw-ordinal: ;--tw-slashed-zero: ;--tw-numeric-figure: ;--tw-numeric-spacing: ;--tw-numeric-fraction: ;--tw-ring-inset: ;--tw-ring-offset-width:0px;--tw-ring-offset-color:#fff;--tw-ring-color:#3b82f680;--tw-ring-offset-shadow:0 0 #0000;--tw-ring-shadow:0 0 #0000;--tw-shadow:0 0 #0000;--tw-shadow-colored:0 0 #0000;--tw-blur: ;--tw-brightness: ;--tw-contrast: ;--tw-grayscale: ;--tw-hue-rotate: ;--tw-invert: ;--tw-saturate: ;--tw-sepia: ;--tw-drop-shadow: ;--tw-backdrop-blur: ;--tw-backdrop-brightness: ;--tw-backdrop-contrast: ;--tw-backdrop-grayscale: ;--tw-backdrop-hue-rotate: ;--tw-backdrop-invert: ;--tw-backdrop-opacity: ;--tw-backdrop-saturate: ;--tw-backdrop-sepia: }.container{width:100%}@media (min-width:640px){.container{max-width:640px}}@media (min-width:768px){.container{max-width:768px}}@media (min-width:1024px){.container{max-width:1024px}}@media (min-width:1280px){.container{max-width:1280px}}@media (min-width:1536px){.container{max-width:1536px}}.fixed{position:fixed}.absolute{position:absolute}.relative{position:relative}.inset-0{inset:0}.mx-auto{margin-left:auto;margin-right:auto}.mt-10{margin-top:2.5rem}.mt-48{margin-top:12rem}.mt-6{margin-top:1.5rem}.mt-8{margin-top:2rem}.block{display:block}.flex{display:flex}.table{display:table}.hidden{display:none}.h-10{height:2.5rem}.h-20{height:5rem}.h-full{height:100%}.h-screen{height:100vh}.min-h-24{min-height:6rem}.w-20{width:5rem}.w-9{width:2.25rem}.w-full{width:100%}.basis-5{flex-basis:1.25rem}.basis-5\/12{flex-basis:41.666667%}.transform{transform:translate(var(--tw-translate-x),var(--tw-translate-y)) rotate(var(--tw-rotate)) skewX(var(--tw-skew-x)) skewY(var(--tw-skew-y)) scaleX(var(--tw-scale-x)) scaleY(var(--tw-scale-y))}.cursor-pointer{cursor:pointer}.resize{resize:both}.flex-wrap{flex-wrap:wrap}.items-center{align-items:center}.justify-center{justify-content:center}.gap-8{gap:2rem}.space-x-4>:not([hidden])~:not([hidden]){--tw-space-x-reverse:0;margin-right:calc(1rem*var(--tw-space-x-reverse));margin-left:calc(1rem*(1 - var(--tw-space-x-reverse)))}.space-y-4>:not([hidden])~:not([hidden]){--tw-space-y-reverse:0;margin-top:calc(1rem*(1 - var(--tw-space-y-reverse)));margin-bottom:calc(1rem*var(--tw-space-y-reverse))}.space-y-6>:not([hidden])~:not([hidden]){--tw-space-y-reverse:0;margin-top:calc(1.5rem*(1 - var(--tw-space-y-reverse)));margin-bottom:calc(1.5rem*var(--tw-space-y-reverse))}.rounded-lg{border-radius:.5rem}.rounded-xl{border-radius:.75rem}.border{border-width:1px}.border-gray-500{--tw-border-opacity:1;border-color:rgb(107 114 128/var(--tw-border-opacity))}.border-zinc-200{--tw-border-opacity:1;border-color:rgb(228 228 231/var(--tw-border-opacity))}.bg-black{--tw-bg-opacity:1;background-color:rgb(0 0 0/var(--tw-bg-opacity))}.bg-blue-700{--tw-bg-opacity:1;background-color:rgb(29 78 216/var(--tw-bg-opacity))}.bg-gray-500\/50{background-color:#6b728080}.bg-white{--tw-bg-opacity:1;background-color:rgb(255 255 255/var(--tw-bg-opacity))}.p-4{padding:1rem}.p-6{padding:1.5rem}.px-3{padding-left:.75rem;padding-right:.75rem}.py-2{padding-top:.5rem;padding-bottom:.5rem}.py-6{padding-top:1.5rem;padding-bottom:1.5rem}.text-2xl{font-size:1.5rem;line-height:2rem}.font-bold{font-weight:700}.font-medium{font-weight:500}.text-black{--tw-text-opacity:1;color:rgb(0 0 0/var(--tw-text-opacity))}.text-white{--tw-text-opacity:1;color:rgb(255 255 255/var(--tw-text-opacity))}.opacity-0{opacity:0}.opacity-100{opacity:1}.opacity-25{opacity:.25}.opacity-50{opacity:.5}.opacity-75{opacity:.75}.outline{outline-style:solid}.ring-offset-white{--tw-ring-offset-color:#fff}.transition{transition-property:color,background-color,border-color,text-decoration-color,fill,stroke,opacity,box-shadow,transform,filter,-webkit-backdrop-filter;transition-property:color,background-color,border-color,text-decoration-color,fill,stroke,opacity,box-shadow,transform,filter,backdrop-filter;transition-property:color,background-color,border-color,text-decoration-color,fill,stroke,opacity,box-shadow,transform,filter,backdrop-filter,-webkit-backdrop-filter;transition-timing-function:cubic-bezier(.4,0,.2,1);transition-duration:.15s}.transition-all{transition-property:all;transition-timing-function:cubic-bezier(.4,0,.2,1);transition-duration:.15s}.duration-500{transition-duration:.5s}.ease-in{transition-timing-function:cubic-bezier(.4,0,1,1)}.toggle,.toggle label{position:relative}.toggle label{display:inline-block;width:80px;height:80px;background-color:#fd1015;border-radius:50px;cursor:pointer;box-shadow:inset 0 0 2px 1px #0000001a,0 9px 15px 0 #ef4247;-webkit-tap-highlight-color:transparent}.toggle label:before{content:'';position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);transition:width .2s cubic-bezier(0,-1.85,.27,1.75);height:42px;width:42px;background-color:#fd0f14;border-radius:46px;box-shadow:inset 0 0 0 8px #fff}.toggle input{display:none}.toggle input:checked+label{background-color:#57de72;box-shadow:inset 0 0 2px 1px #0000001a,0 9px 15px 0 rgba(3,132,28,.541)}.toggle input:checked+label:before{width:10px;background-color:#fff}.on .toggle:after{color:#fff}.on .wrapper{background-color:#57de72}.wrapper{background-color:#fd1015}body{background-color:#f0f7ef}.container{display:flex;width:500px;height:500px;margin:auto;text-alignt:center}.slider{width:100%}input[type=range]{-webkit-appearance:none!important;width:100%;height:15px;border-radius:9999px;border-width:2px;--tw-border-opacity:1;border-color:rgb(156 163 175/var(--tw-border-opacity));--tw-bg-opacity:1;background-color:rgb(156 163 175/var(--tw-bg-opacity));margin:auto;transition:all .3s ease}input[type=range]:hover{--tw-bg-opacity:1;background-color:rgb(209 213 219/var(--tw-bg-opacity))}input[type=range]::-webkit-slider-thumb{-webkit-appearance:none!important;width:35px;height:20px;background-color:#fff;border-radius:30px;box-shadow:0 0 3px #fff;-webkit-transition:all .5s ease;transition:all .5s ease}input[type=range]::-webkit-slider-thumb:hover{--tw-bg-opacity:1;background-color:rgb(209 213 219/var(--tw-bg-opacity))}input[type=range]::-webkit-slider-thumb:active{box-shadow:0 0 1px #d1d5db}#rangevalue{text-align:center;font-family:Quantico,sans-serif;font-size:18px;display:block;margin:auto;padding:10px 0;width:100%;color:#579e81}.file\:border-0::file-selector-button{border-width:0}.file\:bg-transparent::file-selector-button{background-color:initial}.file\:text-sm::file-selector-button{font-size:.875rem;line-height:1.25rem}.file\:font-medium::file-selector-button{font-weight:500}.placeholder\:font-normal::-moz-placeholder{font-weight:400}.placeholder\:font-normal::placeholder{font-weight:400}.focus\:shadow-\[0px_0px_0px_4px_\#C9DDFE\2c 0px_1px_2px_0px_\#C9DDFE0C\]:focus{--tw-shadow:0px 0px 0px 4px #c9ddfe,0px 1px 2px 0px #c9ddfe0c;--tw-shadow-colored:0px 0px 0px 4px var(--tw-shadow-color),0px 1px 2px 0px var(--tw-shadow-color);box-shadow:var(--tw-ring-offset-shadow,0 0 #0000),var(--tw-ring-shadow,0 0 #0000),var(--tw-shadow)}.focus\:outline-0:focus{outline-width:0}.disabled\:cursor-not-allowed:disabled{cursor:not-allowed}.disabled\:bg-gray-100:disabled{--tw-bg-opacity:1;background-color:rgb(243 244 246/var(--tw-bg-opacity))}@media (min-width:1024px){.lg\:mt-0{margin-top:0}.lg\:mt-8{margin-top:2rem}.lg\:w-9\/12{width:75%}.lg\:basis-5\/12{flex-basis:41.666667%}.lg\:space-y-0>:not([hidden])~:not([hidden]){--tw-space-y-reverse:0;margin-top:calc(0px*(1 - var(--tw-space-y-reverse)));margin-bottom:calc(0px*var(--tw-space-y-reverse))}}
    </style>
  </head>
  <body
    class='bg-black h-screen text-white flex items-center justify-center w-full mt-48 lg:mt-0'
  >
    <main class='lg:w-9/12 gap-8 mx-auto p-6'>
      <section
        class='w-full h-full flex flex-wrap gap-8 mx-auto justify-center'
      >
        <div
          class='lg:basis-5/12 w-full flex space-x-4 items-center wrapper p-4 rounded-lg relative'
        >
          <div class='toggle'>
            <input type='checkbox' id='toggle' />
            <label for='toggle'></label>
          </div>
          <div>
            <p>Led State</p>
            <h2 id='led-state' class='text-2xl font-bold'>OFF</h2>
          </div>
        </div>
        <div
          class='lg:basis-5/12 w-full bg-white flex space-x-4 items-center min-h-24 p-4 text-black rounded-lg'
        >
          <div id='mode-switch' class='cursor-pointer'>
            <svg
              class='w-20 h-20'
              viewBox='0 0 24 24'
              fill='none'
              xmlns='http://www.w3.org/2000/svg'
            >
              <path
                d='M3 5.5L5 3.5M21 5.5L19 3.5M12 8.5V12.5L14 14.5M20 12.5C20 16.9183 16.4183 20.5 12 20.5C7.58172 20.5 4 16.9183 4 12.5C4 8.08172 7.58172 4.5 12 4.5C16.4183 4.5 20 8.08172 20 12.5Z'
                stroke='#000000'
                stroke-width='2'
                stroke-linecap='round'
                stroke-linejoin='round'
              />
            </svg>
          </div>
          <div>
            <p>Led mode</p>
            <h2 id='led-mode' class='text-2xl font-bold'>Time Mode</h2>
          </div>
        </div>
        <div
          class='lg:basis-5/12 w-full min-h-24 flex items-center p-4 border opacity-25 space-x-4 border-gray-500 rounded-lg cursor-pointer'
          id='led-brightness'
        >
          <div id='mode-switch' class='cursor-pointer'>
            <svg
              class='w-20 h-20'
              viewBox='0 0 24 24'
              fill='none'
              xmlns='http://www.w3.org/2000/svg'
            >
              <path
                d='M12 0C11.4477 0 11 0.447715 11 1V3C11 3.55228 11.4477 4 12 4C12.5523 4 13 3.55228 13 3V1C13 0.447715 12.5523 0 12 0Z'
                fill='#fff'
              />
              <path
                fill-rule='evenodd'
                clip-rule='evenodd'
                d='M12 18C15.3137 18 18 15.3137 18 12C18 8.68629 15.3137 6 12 6C8.68629 6 6 8.68629 6 12C6 15.3137 8.68629 18 12 18ZM9.21518 14.7848C8.50248 14.0721 8.06167 13.0875 8.06167 12C8.06167 9.82492 9.82492 8.06167 12 8.06167C13.0875 8.06167 14.0721 8.50248 14.7848 9.21518L9.21518 14.7848Z'
                fill='#fff'
              />
              <path
                d='M19.0711 3.51472C19.4616 3.12419 20.0947 3.12419 20.4853 3.51472C20.8758 3.90524 20.8758 4.53841 20.4853 4.92893L19.0711 6.34315C18.6805 6.73367 18.0474 6.73367 17.6568 6.34315C17.2663 5.95262 17.2663 5.31946 17.6568 4.92893L19.0711 3.51472Z'
                fill='#fff'
              />
              <path
                d='M0 12C0 12.5523 0.447715 13 1 13H3C3.55228 13 4 12.5523 4 12C4 11.4477 3.55228 11 3 11H1C0.447715 11 0 11.4477 0 12Z'
                fill='#fff'
              />
              <path
                d='M3.51472 4.92893C3.1242 4.53841 3.1242 3.90524 3.51472 3.51472C3.90525 3.12419 4.53841 3.12419 4.92894 3.51472L6.34315 4.92893C6.73368 5.31946 6.73368 5.95262 6.34315 6.34314C5.95263 6.73367 5.31946 6.73367 4.92894 6.34314L3.51472 4.92893Z'
                fill='#fff'
              />
              <path
                d='M12 20C11.4477 20 11 20.4477 11 21V23C11 23.5523 11.4477 24 12 24C12.5523 24 13 23.5523 13 23V21C13 20.4477 12.5523 20 12 20Z'
                fill='#fff'
              />
              <path
                d='M4.92894 17.6569C5.31946 17.2663 5.95263 17.2663 6.34315 17.6569C6.73368 18.0474 6.73368 18.6805 6.34315 19.0711L4.92894 20.4853C4.53842 20.8758 3.90525 20.8758 3.51473 20.4853C3.1242 20.0948 3.1242 19.4616 3.51473 19.0711L4.92894 17.6569Z'
                fill='#fff'
              />
              <path
                d='M20 12C20 12.5523 20.4477 13 21 13H23C23.5523 13 24 12.5523 24 12C24 11.4477 23.5523 11 23 11H21C20.4477 11 20 11.4477 20 12Z'
                fill='#fff'
              />
              <path
                d='M17.6568 19.0711C17.2663 18.6805 17.2663 18.0474 17.6568 17.6569C18.0474 17.2663 18.6805 17.2663 19.0711 17.6569L20.4853 19.0711C20.8758 19.4616 20.8758 20.0948 20.4853 20.4853C20.0947 20.8758 19.4616 20.8758 19.0711 20.4853L17.6568 19.0711Z'
                fill='#fff'
              />
            </svg>
          </div>
          <div>
            <p>Set Brightness</p>
            <h2 id='led-mode' class='text-2xl font-bold'>Brightness</h2>
          </div>
        </div>
        <div
          class='lg:basis-5/12 w-full bg-gray-500/50 min-h-24 flex items-center space-x-4 p-4 rounded-lg relative'
        >
          <input
            type='color'
            id='led-color'
            class='opacity-0 inset-0 absolute cursor-pointer'
          />
          <label for='led-color'>
            <svg
              fill='#fff'
              class='w-20 h-20'
              id='color_layer'
              viewBox='0 0 36 36'
              xmlns='http://www.w3.org/2000/svg'
            >
              <g id='b894e32b-0437-45ab-b8bf-9f0c4e8f57cd' data-name='Layer 3'>
                <path
                  d='M32.23,14.89c-2.1-.56-4.93,1.8-6.34.3-1.71-1.82,2.27-5.53,1.86-8.92-.33-2.78-3.51-4.08-6.66-4.1A18.5,18.5,0,0,0,7.74,7.59c-6.64,6.59-8.07,16-1.37,22.48,6.21,6,16.61,4.23,22.67-1.4a17.73,17.73,0,0,0,4.22-6.54C34.34,19.23,34.44,15.49,32.23,14.89ZM9.4,10.57a2.23,2.23,0,0,1,2.87,1.21,2.22,2.22,0,0,1-1.81,2.53A2.22,2.22,0,0,1,7.59,13.1,2.23,2.23,0,0,1,9.4,10.57ZM5.07,20.82a2.22,2.22,0,0,1,1.82-2.53A2.22,2.22,0,0,1,9.75,19.5,2.23,2.23,0,0,1,7.94,22,2.24,2.24,0,0,1,5.07,20.82Zm7,8.33a2.22,2.22,0,0,1-2.87-1.21A2.23,2.23,0,0,1,11,25.41a2.23,2.23,0,0,1,2.87,1.21A2.22,2.22,0,0,1,12,29.15ZM15,8.26a2.23,2.23,0,0,1,1.81-2.53,2.24,2.24,0,0,1,2.87,1.21,2.22,2.22,0,0,1-1.82,2.53A2.21,2.21,0,0,1,15,8.26Zm5.82,22.19a2.22,2.22,0,0,1-2.87-1.21,2.23,2.23,0,0,1,1.81-2.53,2.24,2.24,0,0,1,2.87,1.21A2.22,2.22,0,0,1,20.78,30.45Zm5-10.46a3.2,3.2,0,0,1-1.69,1.76,3.53,3.53,0,0,1-1.4.3,2.78,2.78,0,0,1-2.56-1.5,2.49,2.49,0,0,1-.07-2,3.2,3.2,0,0,1,1.69-1.76,3,3,0,0,1,4,1.2A2.54,2.54,0,0,1,25.79,20Z'
                />
              </g>
            </svg>
          </label>
          <div>
            <h2 id='led-mode' class='text-2xl font-bold'>Color</h2>
          </div>
        </div>
      </section>
      <section class='lg:mt-8 mt-6 space-y-6 lg:space-y-4'>
        <div
          class='lg:basis-5/12 w-full bg-gray-500/50 min-h-24 flex items-center space-x-4 p-4 rounded-lg'
        >
          <div>
            <h2 class='text-2xl font-bold'>Speed</h2>
          </div>
          <div class='slider'>
            <input type='range' id='speed-control' min='0' max='100' />
          </div>
        </div>
        <div
          class='basis-5/12 bg-gray-500/50 min-h-24 flex items-center space-x-4 p-4 rounded-lg'
        >
          <h2 class='text-2xl font-bold'>Message</h2>
          <div class='w-full'>
            <input
              type='text'
              class='flex h-10 w-full rounded-lg border border-zinc-200 bg-white px-3 py-2 text-black ring-offset-white file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-grey-550 placeholder:font-normal font-medium text-dark-blue-800 focus:outline-0 focus:shadow-[0px_0px_0px_4px_#C9DDFE,0px_1px_2px_0px_#C9DDFE0C] transition-all duration-500 ease-in disabled:cursor-not-allowed disabled:text-grey-500 disabled:bg-gray-100'
              id='message-input'
            />
          </div>
        </div>
      </section>
      <button
        id='submit-button'
        class='w-full mt-10 bg-blue-700 text-white py-6 rounded-xl'
      >
        Submit
      </button>
    </main>
    <script>
      let color = '#ffffff'; //done
      let mode = 0; // done
      let brightness = 0.25; // done
      let state; // done
      // JUST FOR BACKGROUND CHANGE
      const toggle = document.querySelector('#toggle');
      const ledState = document.querySelector('#led-state');
      const ledMode = document.querySelector('#led-mode');
      const ledColor = document.querySelector('#led-color');
      const ledColorIcon = document.querySelector('#color_layer');
      const messageInput = document.querySelector('#message-input');
      const speedControl = document.querySelector('#speed-control');
      const dateInput = document.querySelector('#date-input');
      const iconText = `
        <svg class='w-20 h-20' viewBox='0 0 24 24' fill='none' xmlns='http://www.w3.org/2000/svg'>
<path d='M11.25 17C11.25 17.4142 11.5858 17.75 12 17.75C12.4142 17.75 12.75 17.4142 12.75 17H11.25ZM15.25 9.75C15.25 10.1642 15.5858 10.5 16 10.5C16.4142 10.5 16.75 10.1642 16.75 9.75H15.25ZM7.25 9.75C7.25 10.1642 7.58579 10.5 8 10.5C8.41421 10.5 8.75 10.1642 8.75 9.75H7.25ZM15.7071 7.32544L16.2646 6.82371V6.82371L15.7071 7.32544ZM9.5 16.25C9.08579 16.25 8.75 16.5858 8.75 17C8.75 17.4142 9.08579 17.75 9.5 17.75V16.25ZM15 17.75C15.4142 17.75 15.75 17.4142 15.75 17C15.75 16.5858 15.4142 16.25 15 16.25V17.75ZM10 7.75H12V6.25H10V7.75ZM12 7.75H14V6.25H12V7.75ZM12.75 17V7H11.25V17H12.75ZM15.25 9.22222V9.75H16.75V9.22222H15.25ZM7.25 9.22222V9.75H8.75V9.22222H7.25ZM14 7.75C14.4949 7.75 14.7824 7.75196 14.9865 7.78245C15.0783 7.79617 15.121 7.8118 15.1376 7.8194C15.148 7.82415 15.1477 7.82503 15.1496 7.82716L16.2646 6.82371C15.96 6.4853 15.579 6.35432 15.2081 6.29891C14.8676 6.24804 14.4479 6.25 14 6.25V7.75ZM16.75 9.22222C16.75 8.71757 16.7513 8.27109 16.708 7.91294C16.6629 7.54061 16.559 7.15082 16.2646 6.82371L15.1496 7.82716C15.1523 7.83015 15.1609 7.83939 15.1731 7.87221C15.1873 7.91048 15.2048 7.97725 15.2188 8.09313C15.2487 8.34011 15.25 8.67931 15.25 9.22222H16.75ZM10 6.25C9.55208 6.25 9.13244 6.24804 8.79192 6.29891C8.42102 6.35432 8.04 6.4853 7.73542 6.82371L8.85036 7.82716C8.85228 7.82503 8.85204 7.82415 8.86242 7.8194C8.87904 7.8118 8.92168 7.79617 9.01354 7.78245C9.21765 7.75196 9.50511 7.75 10 7.75V6.25ZM8.75 9.22222C8.75 8.67931 8.75129 8.34011 8.78118 8.09313C8.7952 7.97725 8.81273 7.91048 8.8269 7.87221C8.83905 7.83939 8.84767 7.83015 8.85036 7.82716L7.73542 6.82371C7.44103 7.15082 7.3371 7.54061 7.29204 7.91294C7.24871 8.27109 7.25 8.71757 7.25 9.22222H8.75ZM9.5 17.75H15V16.25H9.5V17.75Z' fill='#1C274C'/>
<path d='M7 3.33782C8.47087 2.48697 10.1786 2 12 2C17.5228 2 22 6.47715 22 12C22 17.5228 17.5228 22 12 22C6.47715 22 2 17.5228 2 12C2 10.1786 2.48697 8.47087 3.33782 7' stroke='#000000' stroke-width='1.5' stroke-linecap='round'/>
</svg>`;
      const iconTime = `
        <svg
              class='w-20 h-20'
              viewBox='0 0 24 24'
              fill='none'
              xmlns='http://www.w3.org/2000/svg'
            >
              <path
                d='M3 5.5L5 3.5M21 5.5L19 3.5M12 8.5V12.5L14 14.5M20 12.5C20 16.9183 16.4183 20.5 12 20.5C7.58172 20.5 4 16.9183 4 12.5C4 8.08172 7.58172 4.5 12 4.5C16.4183 4.5 20 8.08172 20 12.5Z'
                stroke='#000000'
                stroke-width='2'
                stroke-linecap='round'
                stroke-linejoin='round'
              />
            </svg>
            `;
      const iconTemperature = `
            <svg class='w-20 h-20' viewBox='0 0 24 24' xmlns='http://www.w3.org/2000/svg'>
    <g>
        <path fill='none' d='M0 0h24v24H0z'/>
        <path d='M4.5 10a3.5 3.5 0 1 1 0-7 3.5 3.5 0 0 1 0 7zm0-2a1.5 1.5 0 1 0 0-3 1.5 1.5 0 0 0 0 3zM22 10h-2a4 4 0 1 0-8 0v5a4 4 0 1 0 8 0h2a6 6 0 1 1-12 0v-5a6 6 0 1 1 12 0z'/>
    </g>
</svg>`;
const iconCurrency = `<svg 
  class='w-20 h-20'
  viewBox='-0.5 0 25 25'
  fill='none'
  xmlns='http://www.w3.org/2000/svg'
>
<path d='M12 22.9199C17.5228 22.9199 22 18.4428 22 12.9199C22 7.39707 17.5228 2.91992 12 2.91992C6.47715 2.91992 2 7.39707 2 12.9199C2 18.4428 6.47715 22.9199 12 22.9199Z' stroke='#000000' stroke-width='1.5' stroke-linecap='round' stroke-linejoin='round'/>
<path d='M12.7002 17.1099V18.21C12.7002 18.3877 12.6296 18.5582 12.504 18.6838C12.3783 18.8095 12.2079 18.8799 12.0302 18.8799C11.8525 18.8799 11.6821 18.8095 11.5565 18.6838C11.4308 18.5582 11.3602 18.3877 11.3602 18.21V17.0801C10.9165 17.0072 10.4917 16.8468 10.1106 16.6082C9.72943 16.3695 9.39958 16.0573 9.14023 15.6899C9.04577 15.57 8.99311 15.4226 8.99023 15.27C8.99014 15.1834 9.00762 15.0975 9.04164 15.0178C9.07566 14.9382 9.12551 14.8662 9.18816 14.8064C9.2508 14.7466 9.32494 14.7 9.40608 14.6697C9.48723 14.6393 9.5737 14.6258 9.66023 14.6299C9.74611 14.6294 9.83102 14.648 9.90884 14.6843C9.98667 14.7206 10.0554 14.774 10.1102 14.8401C10.4301 15.258 10.8643 15.574 11.3602 15.75V13.21C10.0302 12.69 9.36023 11.9099 9.36023 10.8999C9.38027 10.3592 9.59279 9.84343 9.95949 9.44556C10.3262 9.04769 10.8229 8.79397 11.3602 8.72998V7.62988C11.3602 7.45219 11.4308 7.2819 11.5565 7.15625C11.6821 7.0306 11.8525 6.95996 12.0302 6.95996C12.2079 6.95996 12.3783 7.0306 12.504 7.15625C12.6296 7.2819 12.7002 7.45219 12.7002 7.62988V8.71997C13.0723 8.77828 13.4289 8.91103 13.7485 9.11035C14.0681 9.30967 14.3442 9.57137 14.5602 9.87988C14.6555 9.99235 14.7117 10.1329 14.7202 10.28C14.7229 10.3657 14.7083 10.451 14.6774 10.531C14.6464 10.611 14.5997 10.684 14.54 10.7456C14.4803 10.8072 14.4088 10.856 14.3298 10.8894C14.2509 10.9228 14.166 10.94 14.0802 10.9399C13.9906 10.9394 13.9022 10.9196 13.8211 10.8816C13.74 10.8436 13.668 10.7884 13.6102 10.72C13.3718 10.4221 13.0574 10.1942 12.7002 10.0601V12.3101L12.9502 12.4099C14.2202 12.9099 15.0102 13.63 15.0102 14.77C14.9954 15.3808 14.7481 15.9629 14.3189 16.3977C13.8897 16.8325 13.3108 17.0871 12.7002 17.1099ZM11.3602 11.73V10.0999C11.1988 10.1584 11.0599 10.2662 10.963 10.408C10.8662 10.5497 10.8162 10.7183 10.8202 10.8899C10.8185 11.0673 10.8688 11.2414 10.9647 11.3906C11.0607 11.5399 11.1981 11.6579 11.3602 11.73ZM13.5502 14.8C13.5502 14.32 13.2202 14.03 12.7002 13.8V15.8C12.9387 15.7639 13.156 15.6427 13.3122 15.459C13.4684 15.2752 13.553 15.0412 13.5502 14.8Z' fill='#000000'/>
</svg>`
      const MODES = ['TIME', 'TEMPERATURE', 'CRYPTO', 'MESSAGE'];
      function hexToRGBA(hex) {
        if (hex) {
          // Remove the hash (#) if it exists
          hex = hex?.replace(/^#/, '');
          // Parse the hex values
          var r = parseInt(hex?.substring(0, 2), 16);
          var g = parseInt(hex?.substring(2, 4), 16);
          var b = parseInt(hex?.substring(4, 6), 16);

          // Ensure values are within the valid range (0-255 for RGB, 0-1 for alpha)
          r = Math.min(255, Math.max(0, r));
          g = Math.min(255, Math.max(0, g));
          b = Math.min(255, Math.max(0, b));

          // Create and return the RGBA string
          return +r + '|' + g + '|' + b;
        } else {
          return undefined;
        }
      }
      function submitConfig(endpoint, query) {
        return fetch(`${endpoint}?${query}`)
          .then((res) => {
            if (res.status === 200) {
              return res.json();
            } else {
              alert('Server Error');
            }
          })
          .then((res) => {
            if (res === '+OK') {
              // Do something
            } else {
              alert('An error occurred!');
            }
          });
      }
      function rgbaToHex(rgba) {
        let parts = rgba.substring(rgba.indexOf('(')).split(',');
        let r = parseInt(parts[0].substring(1)).toString(16),
          g = parseInt(parts[1]).toString(16),
          b = parseInt(parts[2]).toString(16);

        if (r.length == 1) r = '0' + r;
        if (g.length == 1) g = '0' + g;
        if (b.length == 1) b = '0' + b;

        return '#' + r + g + b;
      }
      function handleOnLoad() {
        fetch('/getConfig')
          .then((res) => res.text())
          .then((res) => {
            console.log(res);
            const [
              calledState,
              calledMode,
              calledBrightness,
              speed,
              calledColor,
              message,
            ] = res.split(',');
            state = calledState;
            if (calledState === 'ON') {
              document.body.classList.add('on');
              ledState.innerHTML = 'ON';
              toggle.checked = true;
            } else {
              document.body.classList.remove('on');
              ledState.innerHTML = 'OFF';
              toggle.checked = false;
            }
            
            mode = calledMode;
            modeIndex = calledMode;
            ledMode.innerHTML = MODES[calledMode];
            if (calledMode === 0) {
          modSwitch.innerHTML = iconTime;
        } else if (calledMode === 1) {
          modSwitch.innerHTML = iconTemperature;
        }  else if (calledMode === 2) {
          modSwitch.innerHTML = iconCurrency;
        }
        else {
          modSwitch.innerHTML = iconText;
        }

            brightness = calledBrightness;
            if (calledBrightness == '0.25') {
              ledBrightness.classList.remove('opacity-50');
              ledBrightness.classList.remove('opacity-75');
              ledBrightness.classList.remove('opacity-100');
              ledBrightness.classList.add('opacity-25');
            } else if (calledBrightness == '0.50') {
              ledBrightness.classList.remove('opacity-25');
              ledBrightness.classList.remove('opacity-75');
              ledBrightness.classList.remove('opacity-100');
              ledBrightness.classList.add('opacity-50');
            } else if (calledBrightness == '0.75') {
              ledBrightness.classList.remove('opacity-25');
              ledBrightness.classList.remove('opacity-50');
              ledBrightness.classList.remove('opacity-100');
              ledBrightness.classList.add('opacity-75');
            } else if (calledBrightness == '1.00') {
              ledBrightness.classList.remove('opacity-25');
              ledBrightness.classList.remove('opacity-50');
              ledBrightness.classList.remove('opacity-75');
              ledBrightness.classList.add('opacity-100');
            }
            color = rgbaToHex('(' + calledColor?.replaceAll('|', ',') + ')');
            ledColorIcon.style.fill = rgbaToHex(
              '(' + calledColor?.replaceAll('|', ',') + ')'
            );
            ledColor.value = rgbaToHex(
              '(' + calledColor?.replaceAll('/', ',') + ')'
            );
            messageInput.value = message?.replaceAll('|', ',');
            speedControl.value = speed;
          });
      }
      const ledBrightness = document.querySelector('#led-brightness');
      let modeIndex = 0;
      const modSwitch = document.querySelector('#mode-switch');
      modSwitch.addEventListener('click', () => {
        modeIndex = (modeIndex + 1) % MODES.length;
        ledMode.innerHTML = MODES[modeIndex];
        mode = modeIndex;
        if (modeIndex === 0) {
          modSwitch.innerHTML = iconTime;
        } else if (modeIndex === 1) {
          modSwitch.innerHTML = iconTemperature;
        }  else if (modeIndex === 2) {
          modSwitch.innerHTML = iconCurrency;
        }
        else {
          modSwitch.innerHTML = iconText;
        }
      });

      ledBrightness.addEventListener('click', () => {
        if (ledBrightness.classList.contains('opacity-25')) {
          ledBrightness.classList.remove('opacity-25');
          ledBrightness.classList.add('opacity-50');
          brightness = 0.5;
        } else if (ledBrightness.classList.contains('opacity-50')) {
          ledBrightness.classList.remove('opacity-50');
          ledBrightness.classList.add('opacity-75');
          brightness = 0.75;
        } else if (ledBrightness.classList.contains('opacity-75')) {
          ledBrightness.classList.remove('opacity-75');
          ledBrightness.classList.add('opacity-100');
          brightness = 1;
        } else if (ledBrightness.classList.contains('opacity-100')) {
          ledBrightness.classList.remove('opacity-100');
          ledBrightness.classList.add('opacity-25');
          brightness = 0.25;
        }
      });

      ledColor.addEventListener('change', (e) => {
        console.log(e.target.value);
        ledColorIcon.style.fill = e.target.value;
        color = e.target.value;
      });
      const updateBackground = (event) => {
        document.body.classList.toggle('on');
      };
      async function fetchCoin() {
        const url = 'https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd&x_cg_demo_api_key=CG-mrqtvF7Ez1BpNHtDBnMg52oj';
const options = {
	method: 'GET'
};

try {
	const response = await fetch(url, options);
	const result = await response.json();
	console.log(result);
  return result?.bitcoin?.usd;
} catch (error) {
	console.error(error);
  return undefined;
}
      }

      window.addEventListener('load', async () => {
        handleOnLoad();
      });
      toggle.addEventListener(
        'click',
        () => {
          document.body.classList.toggle('on');
          ledState.innerHTML = document.body.classList.contains('on')
            ? 'ON'
            : 'OFF';
          state = document.body.classList.contains('on') ? 'ON' : 'OFF';
        },
        false
      );
      const submitButton = document.querySelector('#submit-button');
      submitButton.addEventListener('click', (e) => {
        console.log({mode});
        if(mode === 2){
          setInterval(async () => {
            console.log('Fetching....');
            const data = await fetchCoin();
            const coinText = `Bitcoin Stats: the current price of bitcoin is $${data}`;
            submitConfig(
          '/setText',
          `Settings=${state},${mode},${brightness},${
            speedControl.value
          },${hexToRGBA(color)},${messageInput.value.replaceAll(',', '|')},${coinText}`
          );
          console.log('Fetched....');
          }, 15000)
        }
        else{
        submitConfig(
          '/setText',
          `Settings=${state},${mode},${brightness},${
            speedControl.value
          },${hexToRGBA(color)},${messageInput.value.replaceAll(',', '|')},undefined`
        );
      }
      });
    </script>
  </body>
</html>
)=====";
const char* ssid = "ESP32_WS";
const char* password = "iloveengineering";
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
String display_Modes = "";
String single_Row_Txt = "";
int INC_SPD = 50;
WebServer server(80);  

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
  }
void handleSwitch(String state){
    prefs.putString("state", state);
}
void handleMode(int32_t mode){
prefs.putInt("mode", mode);
}
void handleBrightness(double_t brightness){
  prefs.putDouble("brightness", brightness);
}
void handleSpeed(int32_t speed){
  prefs.putInt("speed", speed);
}
void handleMessage(String message){
  prefs.putString("message", message);
}
void handleCoinData(String data){
  prefs.putString("coindata", data);
}
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void handleColor(String color){
  prefs.putString("color", color);
  matrix.setTextColor(matrix.Color(getValue(prefs.getString("color"), '|', 1).toInt(), getValue(prefs.getString("color"), '|', 0).toInt(), getValue(prefs.getString("color"), '|', 2).toInt()));
}
String replaceString(String data, char separator, char replacement){
  for(int i = 0; i < data.length(); i++){
    if(data.charAt(i) == separator){
      data[i] = replacement;
    }
  }
  return data;
}
void handleSettings() {
  String incoming_Settings = server.arg("Settings");
  Serial.println();
  Serial.println("Incoming settings : ");
  Serial.println(incoming_Settings);
  if (getValue(incoming_Settings, ',', 0)) {
      handleSwitch(getValue(incoming_Settings, ',', 0));
      delay(500);
  }
  if(getValue(incoming_Settings, ',', 1) != "undefined"){
    handleMode(getValue(incoming_Settings, ',', 1).toInt());
  }
  if(getValue(incoming_Settings, ',', 2) != "undefined"){
    handleBrightness(getValue(incoming_Settings, ',', 2).toDouble());
  }
  if(getValue(incoming_Settings, ',', 3) != "undefined"){
    handleSpeed(getValue(incoming_Settings, ',', 3).toInt());
  }
  if(getValue(incoming_Settings, ',', 4) != "undefined"){
    handleColor(getValue(incoming_Settings, ',', 4));
  }
  if(getValue(incoming_Settings, ',', 5) != "undefined"){
    handleMessage(getValue(incoming_Settings, ',', 5));
  }
  if(getValue(incoming_Settings, ',', 6) != "undefined"){
    handleCoinData(getValue(incoming_Settings, ',', 6));
  }
  server.send(200, "text/plane", "+OK");  //--> Sending replies to the client.
  delay(500);
}
void handleGetConfig(){
  String config = prefs.getString("state")+","+prefs.getInt("mode")+","+prefs.getDouble("brightness")+","+prefs.getInt("speed")+","+prefs.getString("color")+","+prefs.getString("message");
  server.send(200, "text/plane", config);  //--> Sending replies to the client.
  delay(500);
} 
void setup(void){
   Serial.begin(115200);
  prefs.begin("my-app", false);
  delay(1000);
    dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  delayMS = sensor.min_delay / 1000;

Serial.println(rtc.begin());
Serial.println(rtc.isrunning());
if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    rtc.adjust(DateTime(2024, 1, 31, 15, 10, 0));
  }
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(0.5);
  matrix.setBrightness(255);
  matrix.setTextColor(colors[0]);
  delay(500);
Serial.println();
Serial.println("Connecting to : ");
Serial.println();
Serial.println("WIFI mode : AP");
WiFi.mode(WIFI_AP);
Serial.println("Setting up ESP32 to be an Access Point.");
WiFi.softAP(ssid, password); //--> Creating Access Points
delay(1000);
Serial.println("Setting up ESP32 softAPConfig.");
WiFi.softAPConfig(local_ip, gateway, subnet);
Serial.println();
Serial.print("SSID name : ");
Serial.println(ssid);
Serial.print("IP address : ");
Serial.println(WiFi.softAPIP());
Serial.println();
Serial.println("Connect your computer or mobile Wifi to the SSID above.");
Serial.println("Visit the IP Address above in your browser to open the main page.");
Serial.println();
delay(500);
  server.on("/", handleRoot); 
  server.on("/setText", handleSettings);
  server.on("/getConfig", handleGetConfig);
  server.begin(); 
  Serial.println();
  Serial.println("HTTP server started");
  prefs.putString("message", "Hello World");
  prefs.putDouble("brightness", 0.25);
  prefs.putInt("speed", 50);
  prefs.putInt("mode", 0);
  prefs.putString("color", "0|255|0");
  prefs.putString("state", "ON");
  delay(500);
}
//________________________________________________________________________________

int x = matrix.width();
int pass = 0;

void handleLed(){
  if(prefs.getString("state") == "ON"){
    matrix.fillScreen(prefs.getDouble("brightness") * 2);
    matrix.setCursor(x, 0);
    String message = replaceString(prefs.getString("message"), '|', ',');
    int condition = -(message.length() * 6 + matrix.width());
    if(prefs.getInt("mode") == 3){
    matrix.print(F(message.c_str()));
    }
    else if(prefs.getInt("mode") == 2){
      String coinData = replaceString(prefs.getString("coindata"), '|', ',');
      condition = -(coinData.length() * 6 + matrix.width());
      matrix.print(F(coinData.c_str()));
    }
    else if(prefs.getInt("mode") == 1){
      sensors_event_t event;
      dht.humidity().getEvent(&event);
      dht.temperature().getEvent(&event);
      Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
      String temp = "Temperature: " + String(event.temperature)+" Celcius Humidity: "+String(event.relative_humidity)+"%";
      condition = -(temp.length() * 6 + matrix.width());
      // Get temperature event and print its value.
      matrix.print(F(temp.c_str()));
    }
    else if(prefs.getInt("mode") == 0){
      DateTime now = rtc.now();
        String line =
     String(now.year())+"-"+
     String(now.month())+"-"+
     String(now.day())+" "+
     String(now.hour())+":"+
     String(now.minute())+":"+
     String(now.second());
      matrix.print(F(line.c_str()));
      condition = -(line.length() * 6 + matrix.width());
    }
    if(--x < condition ) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    if(prefs.getString("color")){
    matrix.setTextColor(matrix.Color(getValue(prefs.getString("color"), '|', 0).toInt(), getValue(prefs.getString("color"), '|', 1).toInt(), getValue(prefs.getString("color"), '|', 2).toInt()));
    }else{
    matrix.setTextColor(colors[pass]);
    }
  }
  matrix.show();
  delay(prefs.getInt("speed"));
  }else{
    matrix.fillScreen(0);
    matrix.show();
  }
}
void loop(void){
server.handleClient();  
handleLed();
}
