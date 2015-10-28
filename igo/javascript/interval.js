var fps = 100;
var charsperword = 4.5;

$(function() {
    $('#speed').change(function() {
        fps = 1000 / (parseInt($(this).val()) / charsperword) * 60; // 最後の6は1語6文字という意味
        console.log(fps);
    });
});
