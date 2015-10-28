var id;
$(function() {
    $('#start').click(function() {
        $('#speed').change();
        var a = $('#in').val().split(" ");
        // 100ms毎に繰り返し
        if (id) clearInterval(id);
        t = 0;
        sumlen = 0;
        id = setInterval(function() {
            var lentoorp = [0, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5];
            if (t >= a.length) {
                $('#score').html(sumlen / (t / (1000/fps)) * 60);
                return;
            }
            a_first = a[t].substring(0, lentoorp[a[t].length]-1);
            a_middle = a[t].substring(lentoorp[a[t].length]-1, lentoorp[a[t].length]);
            a_last = a[t].substring(lentoorp[a[t].length], a[t].length);
            $('#change').html(a_first + '<font color="red">' + a_middle + '</font>' + a_last);
            var o = $('#change').offset();
            o.left = 100 - strWidth(a_first) - strWidth(a_middle) / 2;
            $('#change').offset(o);
            t++;
            sumlen+=a[t].length;
        }, fps);
    });
});

