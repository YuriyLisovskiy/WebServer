$(document).ready(function () {
        $('.content').scroll(function () {
            if ($(this).scrollTop() > 50) { $('#back-to-top').fadeIn(); } else { $('#back-to-top').fadeOut(); }
        });
        $('#back-to-top').click(function () {
            $('#back-to-top').tooltip('hide'); $('.content').animate({ scrollTop: 0 }, 800);
            return false;
        });
        $('#back-to-top').tooltip('show');
	});