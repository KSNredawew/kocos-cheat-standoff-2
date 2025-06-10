import frida, sys

def on_message(message, data):
    if message['type'] == 'send':
        print(f"[*] {message['payload']}")

jscode = """
Interceptor.attach(Module.findExportByName("libGuardian.so", "detect_hooks"), {
    onEnter: function(args) {
        this.result = ptr(0);
    },
    onLeave: function(retval) {
        retval.replace(this.result);
    }
});
"""

process = frida.get_usb_device().attach('Standoff 2')
script = process.create_script(jscode)
script.on('message', on_message)
script.load()
sys.stdin.read()
