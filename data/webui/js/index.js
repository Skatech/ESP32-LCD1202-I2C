function send_onclick() {
    const line1 = document.getElementById("line1").value
    const line2 = document.getElementById("line2").value
    
    const rq = new XMLHttpRequest()
    rq.open('POST', 'set-text', true)
    rq.setRequestHeader("Content-Type", "application/x-www-form-urlencoded")
    rq.onreadystatechange = function() {
        if (rq.readyState === 4) {
            console.log("data sended successfully")
        }
    }
    rq.send('line1=' + line1 + "&line2=" + line2)
}

function connect_onclick() {
}

