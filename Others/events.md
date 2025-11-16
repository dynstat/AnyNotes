```javascript
//--------------------

// Get the select element
const select = document.getElementById('mySelect');

// Click event for the select element
select.addEventListener('click', function() {
  if (this.classList.contains('open')) {
    if (this.value !== 'select token') {
      alert(this.value);
    }
    this.classList.remove('open');
  } else {
    this.classList.add('open');
  }
});

// Blur event for the select element
select.addEventListener('blur', function() {
  this.classList.remove('open');
});

// Click event for the entire document
document.addEventListener('click', function(e) {
  if (!select.contains(e.target)) {
    select.classList.remove('open');
  }
}); 
```

..
