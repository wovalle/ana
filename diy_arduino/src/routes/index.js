var express = require('express');
var router = express.Router();

router.get('/', function(req, res, next) {
    res.json({ status: 'online' });
});

module.exports = router;
