Absolutely! Here’s a **comprehensive, detailed, and modern guide** to all major methods for each Rust collection (from the standard library), including how to **safely retrieve values** (using `match`, `unwrap`, `unwrap_or`, etc.), and **concise code snippets** for each method.  
**All code is commented with input/output types and best practices.**  
Formatting is kept clear and tabular for quick reference.

---

# 1. `Vec<T>` (Growable Vector)

A contiguous, growable array type.

## All Common Methods

| Method          | Signature & Return Type                            | Description / Example                                               |      |               |
| --------------- | -------------------------------------------------- | ------------------------------------------------------------------- | ---- | ------------- |
| `new`           | `fn new() -> Vec<T>`                               | Create empty vector<br>`let v: Vec<i32> = Vec::new();`              |      |               |
| `with_capacity` | `fn with_capacity(usize) -> Vec<T>`                | Preallocate<br>`let v = Vec::with_capacity(10);`                    |      |               |
| `push`          | `fn push(&mut self, T)`                            | Add element<br>`v.push(5);`                                         |      |               |
| `pop`           | `fn pop(&mut self) -> Option<T>`                   | Remove last<br>`let x = v.pop(); // Option<T>`                      |      |               |
| `get`           | `fn get(&self, usize) -> Option<&T>`               | Safe access<br>`let x = v.get(2); // Option<&T>`                    |      |               |
| `get_mut`       | `fn get_mut(&mut self, usize) -> Option<&mut T>`   | Safe mutable access<br>`if let Some(x) = v.get_mut(1) { *x = 10; }` |      |               |
| `remove`        | `fn remove(&mut self, usize) -> T`                 | Remove at index<br>`let x = v.remove(0);`                           |      |               |
| `insert`        | `fn insert(&mut self, usize, T)`                   | Insert at index<br>`v.insert(1, 99);`                               |      |               |
| `len`           | `fn len(&self) -> usize`                           | Length<br>`let n = v.len();`                                        |      |               |
| `is_empty`      | `fn is_empty(&self) -> bool`                       | Check empty<br>`if v.is_empty() {}`                                 |      |               |
| `clear`         | `fn clear(&mut self)`                              | Remove all<br>`v.clear();`                                          |      |               |
| `contains`      | `fn contains(&self, &T) -> bool`                   | Check for value<br>`v.contains(&5);`                                |      |               |
| `iter`          | `fn iter(&self) -> Iter<T>`                        | Iterate<br>`for x in v.iter() {}`                                   |      |               |
| `iter_mut`      | `fn iter_mut(&mut self) -> IterMut<T>`             | Mutable iterate<br>`for x in v.iter_mut() { *x += 1; }`             |      |               |
| `as_slice`      | `fn as_slice(&self) -> &[T]`                       | Get slice<br>`let s: &[i32] = v.as_slice();`                        |      |               |
| `first`         | `fn first(&self) -> Option<&T>`                    | First element<br>`let x = v.first();`                               |      |               |
| `last`          | `fn last(&self) -> Option<&T>`                     | Last element<br>`let x = v.last();`                                 |      |               |
| `swap`          | `fn swap(&mut self, usize, usize)`                 | Swap elements<br>`v.swap(0, 1);`                                    |      |               |
| `reverse`       | `fn reverse(&mut self)`                            | Reverse<br>`v.reverse();`                                           |      |               |
| `sort`          | `fn sort(&mut self)`                               | Sort (requires `T: Ord`)<br>`v.sort();`                             |      |               |
| `sort_by`       | `fn sort_by(&mut self, F)`                         | Custom sort<br>`v.sort_by(                                          | a, b | b.cmp(a));`   |
| `resize`        | `fn resize(&mut self, usize, T)`                   | Resize<br>`v.resize(10, 0);`                                        |      |               |
| `truncate`      | `fn truncate(&mut self, usize)`                    | Shorten<br>`v.truncate(3);`                                         |      |               |
| `retain`        | `fn retain<F>(&mut self, F)`                       | Keep if predicate<br>`v.retain(                                     | &x   | x % 2 == 0);` |
| `dedup`         | `fn dedup(&mut self)`                              | Remove consecutive duplicates<br>`v.dedup();`                       |      |               |
| `split_at`      | `fn split_at(&self, usize) -> (&[T], &[T])`        | Split into two slices<br>`let (a, b) = v.split_at(2);`              |      |               |
| `clone`         | `fn clone(&self) -> Vec<T>`                        | Clone<br>`let v2 = v.clone();`                                      |      |               |
| `extend`        | `fn extend<I: IntoIterator<Item=T>>(&mut self, I)` | Extend from iterator<br>`v.extend([1,2,3]);`                        |      |               |
| `drain`         | `fn drain(Range) -> Drain<T>`                      | Remove range<br>`for x in v.drain(1..3) {}`                         |      |               |
| `capacity`      | `fn capacity(&self) -> usize`                      | Allocated size<br>`let c = v.capacity();`                           |      |               |
| `shrink_to_fit` | `fn shrink_to_fit(&mut self)`                      | Reduce capacity<br>`v.shrink_to_fit();`                             |      |               |

### Safe Retrieval Example

```rust
let v = vec![1, 2, 3];
// Safe access
match v.get(1) {
    Some(val) => println!("Value: {}", val), // val: &i32
    None => println!("No value at index"),
}
// Unsafe (panics if out of bounds)
let x = v[1]; // i32
// Unwrap with default
let y = v.get(10).unwrap_or(&-1); // &i32
```

---

# 2. `String` (Growable UTF-8 Text)

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> String`                                   | Empty string<br>`let s = String::new();`                                                              |
| `from`                | `fn from<T: AsRef<str>>(T) -> String`                  | From &str<br>`let s = String::from("abc");`                                                           |
| `push_str`            | `fn push_str(&mut self, &str)`                         | Append string<br>`s.push_str("def");`                                                                 |
| `push`                | `fn push(&mut self, char)`                             | Append char<br>`s.push('!');`                                                                         |
| `len`                 | `fn len(&self) -> usize`                               | Length in bytes<br>`let n = s.len();`                                                                 |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`s.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`s.clear();`                                                                            |
| `replace`             | `fn replace(&self, &str, &str) -> String`              | Replace substring<br>`let s2 = s.replace("a", "b");`                                                  |
| `contains`            | `fn contains(&self, &str) -> bool`                     | Check substring<br>`s.contains("abc");`                                                               |
| `as_str`              | `fn as_str(&self) -> &str`                             | Get &str<br>`let slice: &str = s.as_str();`                                                           |
| `chars`               | `fn chars(&self) -> Chars`                             | Iterate chars<br>`for c in s.chars() {}`                                                              |
| `bytes`               | `fn bytes(&self) -> Bytes`                             | Iterate bytes<br>`for b in s.bytes() {}`                                                              |
| `split`               | `fn split(&self, &str) -> Split`                       | Split by pattern<br>`for part in s.split(" ") {}`                                                     |
| `trim`                | `fn trim(&self) -> &str`                               | Remove whitespace<br>`let t = s.trim();`                                                              |
| `to_uppercase`        | `fn to_uppercase(&self) -> String`                     | Uppercase<br>`let up = s.to_uppercase();`                                                             |
| `to_lowercase`        | `fn to_lowercase(&self) -> String`                     | Lowercase<br>`let lo = s.to_lowercase();`                                                             |
| `starts_with`         | `fn starts_with(&self, &str) -> bool`                  | Prefix<br>`s.starts_with("He");`                                                                      |
| `ends_with`           | `fn ends_with(&self, &str) -> bool`                    | Suffix<br>`s.ends_with("!");`                                                                         |
| `find`                | `fn find(&self, &str) -> Option<usize>`                | Find substring<br>`let idx = s.find("abc");`                                                          |
| `get`                 | `fn get(&self, Range) -> Option<&str>`                 | Safe slice<br>`let sub = s.get(0..2);`                                                                |
| `insert`              | `fn insert(&mut self, usize, char)`                    | Insert char<br>`s.insert(1, 'X');`                                                                    |
| `insert_str`          | `fn insert_str(&mut self, usize, &str)`                | Insert string<br>`s.insert_str(1, "abc");`                                                            |
| `remove`              | `fn remove(&mut self, usize) -> char`                  | Remove char<br>`let c = s.remove(0);`                                                                 |
| `pop`                 | `fn pop(&mut self) -> Option<char>`                    | Remove last<br>`let c = s.pop();`                                                                     |
| `split_off`           | `fn split_off(&mut self, usize) -> String`             | Split at index<br>`let s2 = s.split_off(3);`                                                          |
| `capacity`            | `fn capacity(&self) -> usize`                          | Allocated size<br>`let c = s.capacity();`                                                             |
| `reserve`             | `fn reserve(&mut self, usize)`                         | Reserve space<br>`s.reserve(10);`                                                                     |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                          | Reduce capacity<br>`s.shrink_to_fit();`                                                               |

### Safe Retrieval Example

```rust
let s = String::from("Hello");
// Safe substring
if let Some(sub) = s.get(0..2) { // sub: &str
    println!("{}", sub);
} else {
    println!("Out of bounds");
}
// Find substring
match s.find("ll") {
    Some(idx) => println!("Found at {}", idx),
    None => println!("Not found"),
}
```

---

# 3. `HashMap<K, V>`

A hash map for key-value pairs.

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> HashMap<K, V>`                            | Empty map<br>`let m: HashMap<i32, i32> = HashMap::new();`                                             |
| `with_capacity`       | `fn with_capacity(usize) -> HashMap<K, V>`             | Preallocate<br>`let m = HashMap::with_capacity(10);`                                                  |
| `insert`              | `fn insert(&mut self, K, V) -> Option<V>`              | Insert key-value<br>`m.insert("a", 1);`                                                               |
| `get`                 | `fn get(&self, &K) -> Option<&V>`                      | Get value<br>`let v = m.get(&"a");`                                                                   |
| `get_mut`             | `fn get_mut(&mut self, &K) -> Option<&mut V>`          | Mutable get<br>`if let Some(v) = m.get_mut(&"a") { *v = 2; }`                                         |
| `remove`              | `fn remove(&mut self, &K) -> Option<V>`                | Remove key<br>`m.remove(&"a");`                                                                       |
| `contains_key`        | `fn contains_key(&self, &K) -> bool`                   | Check key<br>`m.contains_key(&"a");`                                                                  |
| `len`                 | `fn len(&self) -> usize`                               | Number of entries<br>`let n = m.len();`                                                               |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`m.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`m.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<K, V>`                         | Iterate<br>`for (k, v) in m.iter() {}`                                                                |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<K, V>`              | Mutable iterate<br>`for (k, v) in m.iter_mut() { *v += 1; }`                                          |
| `keys`                | `fn keys(&self) -> Keys<K, V>`                         | Iterate keys<br>`for k in m.keys() {}`                                                                |
| `values`              | `fn values(&self) -> Values<K, V>`                     | Iterate values<br>`for v in m.values() {}`                                                            |
| `entry`               | `fn entry(&mut self, K) -> Entry<K, V>`                | Entry API<br>`m.entry("a").or_insert(0);`                                                             |
| `drain`               | `fn drain(&mut self) -> Drain<K, V>`                   | Remove all, iterate<br>`for (k, v) in m.drain() {}`                                                   |
| `retain`              | `fn retain<F>(&mut self, F)`                           | Keep if predicate<br>`m.retain(|k, v| *v > 0);`                                                       |
| `clone`               | `fn clone(&self) -> HashMap<K, V>`                     | Clone<br>`let m2 = m.clone();`                                                                        |
| `extend`              | `fn extend<I: IntoIterator<Item=(K, V)>>(&mut self, I)`| Extend<br>`m.extend(vec![("b", 2)]);`                                                                 |
| `capacity`            | `fn capacity(&self) -> usize`                          | Allocated size<br>`let c = m.capacity();`                                                             |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                          | Reduce capacity<br>`m.shrink_to_fit();`                                                               |

### Safe Retrieval Example

```rust
use std::collections::HashMap;
let mut m = HashMap::new();
m.insert("a", 1);
// Safe get
match m.get(&"a") {
    Some(&v) => println!("Value: {}", v), // v: i32
    None => println!("Not found"),
}
// Unwrap with default
let v = m.get(&"b").unwrap_or(&-1); // &i32
```

---

# 4. `HashSet<T>`

A set of unique values.

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> HashSet<T>`                               | Empty set<br>`let s: HashSet<i32> = HashSet::new();`                                                  |
| `with_capacity`       | `fn with_capacity(usize) -> HashSet<T>`                | Preallocate<br>`let s = HashSet::with_capacity(10);`                                                  |
| `insert`              | `fn insert(&mut self, T) -> bool`                      | Add value<br>`s.insert(5);`                                                                           |
| `contains`            | `fn contains(&self, &T) -> bool`                       | Check value<br>`s.contains(&5);`                                                                      |
| `remove`              | `fn remove(&mut self, &T) -> bool`                     | Remove value<br>`s.remove(&5);`                                                                       |
| `len`                 | `fn len(&self) -> usize`                               | Number of elements<br>`let n = s.len();`                                                              |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`s.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`s.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<T>`                            | Iterate<br>`for x in s.iter() {}`                                                                     |
| `drain`               | `fn drain(&mut self) -> Drain<T>`                      | Remove all, iterate<br>`for x in s.drain() {}`                                                        |
| `retain`              | `fn retain<F>(&mut self, F)`                           | Keep if predicate<br>`s.retain(|&x| x > 0);`                                                          |
| `clone`               | `fn clone(&self) -> HashSet<T>`                        | Clone<br>`let s2 = s.clone();`                                                                        |
| `extend`              | `fn extend<I: IntoIterator<Item=T>>(&mut self, I)`     | Extend<br>`s.extend([1,2,3]);`                                                                        |
| `capacity`            | `fn capacity(&self) -> usize`                          | Allocated size<br>`let c = s.capacity();`                                                             |
| `shrink_to_fit`       | `fn shrink_to_fit(&mut self)`                          | Reduce capacity<br>`s.shrink_to_fit();`                                                               |
| `intersection`        | `fn intersection(&self, &HashSet<T>) -> Intersection`  | Set intersection<br>`for x in s1.intersection(&s2) {}`                                                |
| `union`               | `fn union(&self, &HashSet<T>) -> Union`                | Set union<br>`for x in s1.union(&s2) {}`                                                              |
| `difference`          | `fn difference(&self, &HashSet<T>) -> Difference`      | Set difference<br>`for x in s1.difference(&s2) {}`                                                    |
| `symmetric_difference`| `fn symmetric_difference(&self, &HashSet<T>) -> SymDiff`| Symmetric diff<br>`for x in s1.symmetric_difference(&s2) {}`                                          |
| `is_subset`           | `fn is_subset(&self, &HashSet<T>) -> bool`             | Subset<br>`s1.is_subset(&s2);`                                                                        |
| `is_superset`         | `fn is_superset(&self, &HashSet<T>) -> bool`           | Superset<br>`s1.is_superset(&s2);`                                                                    |

### Safe Retrieval Example

```rust
use std::collections::HashSet;
let mut s = HashSet::new();
s.insert(1);
if s.contains(&1) {
    println!("Set has 1");
}
```

---

# 5. `VecDeque<T>`

A double-ended queue.

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> VecDeque<T>`                              | Empty deque<br>`let d: VecDeque<i32> = VecDeque::new();`                                              |
| `with_capacity`       | `fn with_capacity(usize) -> VecDeque<T>`               | Preallocate<br>`let d = VecDeque::with_capacity(10);`                                                 |
| `push_back`           | `fn push_back(&mut self, T)`                           | Add to back<br>`d.push_back(1);`                                                                      |
| `push_front`          | `fn push_front(&mut self, T)`                          | Add to front<br>`d.push_front(0);`                                                                    |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                  | Remove from back<br>`let x = d.pop_back();`                                                           |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                 | Remove from front<br>`let x = d.pop_front();`                                                         |
| `get`                 | `fn get(&self, usize) -> Option<&T>`                   | Safe access<br>`let x = d.get(1);`                                                                    |
| `get_mut`             | `fn get_mut(&mut self, usize) -> Option<&mut T>`       | Mutable access<br>`if let Some(x) = d.get_mut(0) { *x = 10; }`                                        |
| `len`                 | `fn len(&self) -> usize`                               | Length<br>`let n = d.len();`                                                                          |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`d.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`d.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<T>`                            | Iterate<br>`for x in d.iter() {}`                                                                     |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<T>`                 | Mutable iterate<br>`for x in d.iter_mut() { *x += 1; }`                                               |
| `as_slices`           | `fn as_slices(&self) -> (&[T], &[T])`                  | Get slices<br>`let (a, b) = d.as_slices();`                                                           |
| `front`               | `fn front(&self) -> Option<&T>`                        | First<br>`let x = d.front();`                                                                         |
| `back`                | `fn back(&self) -> Option<&T>`                         | Last<br>`let x = d.back();`                                                                           |
| `make_contiguous`     | `fn make_contiguous(&mut self) -> &mut [T]`            | Make contiguous<br>`let slice = d.make_contiguous();`                                                 |
| `rotate_left`         | `fn rotate_left(&mut self, usize)`                     | Rotate left<br>`d.rotate_left(1);`                                                                    |
| `rotate_right`        | `fn rotate_right(&mut self, usize)`                    | Rotate right<br>`d.rotate_right(1);`                                                                  |

### Safe Retrieval Example

```rust
use std::collections::VecDeque;
let mut d = VecDeque::new();
d.push_back(1);
if let Some(x) = d.get(0) {
    println!("First: {}", x);
}
```

---

# 6. `BTreeMap<K, V>`

A sorted map (ordered by key).

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BTreeMap<K, V>`                           | Empty map<br>`let m: BTreeMap<i32, i32> = BTreeMap::new();`                                           |
| `insert`              | `fn insert(&mut self, K, V) -> Option<V>`              | Insert<br>`m.insert(1, "a");`                                                                         |
| `get`                 | `fn get(&self, &K) -> Option<&V>`                      | Get<br>`let v = m.get(&1);`                                                                           |
| `get_mut`             | `fn get_mut(&mut self, &K) -> Option<&mut V>`          | Mutable get<br>`if let Some(v) = m.get_mut(&1) { *v = "b"; }`                                         |
| `remove`              | `fn remove(&mut self, &K) -> Option<V>`                | Remove<br>`m.remove(&1);`                                                                             |
| `contains_key`        | `fn contains_key(&self, &K) -> bool`                   | Check key<br>`m.contains_key(&1);`                                                                    |
| `len`                 | `fn len(&self) -> usize`                               | Length<br>`let n = m.len();`                                                                          |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`m.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`m.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<K, V>`                         | Iterate<br>`for (k, v) in m.iter() {}`                                                                |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<K, V>`              | Mutable iterate<br>`for (k, v) in m.iter_mut() { *v = "c"; }`                                         |
| `range`               | `fn range<R>(&self, R) -> Range<K, V>`                 | Range query<br>`for (k, v) in m.range(1..5) {}`                                                       |
| `first_key_value`     | `fn first_key_value(&self) -> Option<(&K, &V)>`        | First entry<br>`let x = m.first_key_value();`                                                         |
| `last_key_value`      | `fn last_key_value(&self) -> Option<(&K, &V)>`         | Last entry<br>`let x = m.last_key_value();`                                                           |
| `split_off`           | `fn split_off(&mut self, &K) -> BTreeMap<K, V>`        | Split at key<br>`let m2 = m.split_off(&3);`                                                           |
| `drain_filter`        | `fn drain_filter<F>(&mut self, F) -> DrainFilter<K,V>` | Remove by predicate (nightly)<br>`for (k, v) in m.drain_filter(|k, v| *k > 2) {}`                     |

### Safe Retrieval Example

```rust
use std::collections::BTreeMap;
let mut m = BTreeMap::new();
m.insert(1, "a");
if let Some(v) = m.get(&1) {
    println!("Value: {}", v);
}
```

---

# 7. `BTreeSet<T>`

A sorted set.

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BTreeSet<T>`                              | Empty set<br>`let s: BTreeSet<i32> = BTreeSet::new();`                                                |
| `insert`              | `fn insert(&mut self, T) -> bool`                      | Add value<br>`s.insert(5);`                                                                           |
| `contains`            | `fn contains(&self, &T) -> bool`                       | Check value<br>`s.contains(&5);`                                                                      |
| `remove`              | `fn remove(&mut self, &T) -> bool`                     | Remove value<br>`s.remove(&5);`                                                                       |
| `len`                 | `fn len(&self) -> usize`                               | Length<br>`let n = s.len();`                                                                          |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`s.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`s.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<T>`                            | Iterate<br>`for x in s.iter() {}`                                                                     |
| `range`               | `fn range<R>(&self, R) -> Range<T>`                    | Range query<br>`for x in s.range(1..5) {}`                                                            |
| `first`               | `fn first(&self) -> Option<&T>`                        | First<br>`let x = s.first();`                                                                         |
| `last`                | `fn last(&self) -> Option<&T>`                         | Last<br>`let x = s.last();`                                                                           |
| `split_off`           | `fn split_off(&mut self, &T) -> BTreeSet<T>`           | Split at value<br>`let s2 = s.split_off(&3);`                                                         |
| `is_subset`           | `fn is_subset(&self, &BTreeSet<T>) -> bool`            | Subset<br>`s1.is_subset(&s2);`                                                                        |
| `is_superset`         | `fn is_superset(&self, &BTreeSet<T>) -> bool`          | Superset<br>`s1.is_superset(&s2);`                                                                    |
| `intersection`        | `fn intersection(&self, &BTreeSet<T>) -> Intersection` | Set intersection<br>`for x in s1.intersection(&s2) {}`                                                |
| `union`               | `fn union(&self, &BTreeSet<T>) -> Union`               | Set union<br>`for x in s1.union(&s2) {}`                                                              |
| `difference`          | `fn difference(&self, &BTreeSet<T>) -> Difference`     | Set difference<br>`for x in s1.difference(&s2) {}`                                                    |
| `symmetric_difference`| `fn symmetric_difference(&self, &BTreeSet<T>) -> SymDiff`| Symmetric diff<br>`for x in s1.symmetric_difference(&s2) {}`                                          |

### Safe Retrieval Example

```rust
use std::collections::BTreeSet;
let mut s = BTreeSet::new();
s.insert(1);
if let Some(x) = s.first() {
    println!("First: {}", x);
}
```

---

# 8. `LinkedList<T>`

A doubly-linked list.

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> LinkedList<T>`                            | Empty list<br>`let l: LinkedList<i32> = LinkedList::new();`                                           |
| `push_back`           | `fn push_back(&mut self, T)`                           | Add to back<br>`l.push_back(1);`                                                                      |
| `push_front`          | `fn push_front(&mut self, T)`                          | Add to front<br>`l.push_front(0);`                                                                    |
| `pop_back`            | `fn pop_back(&mut self) -> Option<T>`                  | Remove from back<br>`let x = l.pop_back();`                                                           |
| `pop_front`           | `fn pop_front(&mut self) -> Option<T>`                 | Remove from front<br>`let x = l.pop_front();`                                                         |
| `len`                 | `fn len(&self) -> usize`                               | Length<br>`let n = l.len();`                                                                          |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`l.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`l.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<T>`                            | Iterate<br>`for x in l.iter() {}`                                                                     |
| `iter_mut`            | `fn iter_mut(&mut self) -> IterMut<T>`                 | Mutable iterate<br>`for x in l.iter_mut() { *x += 1; }`                                               |
| `front`               | `fn front(&self) -> Option<&T>`                        | First<br>`let x = l.front();`                                                                         |
| `back`                | `fn back(&self) -> Option<&T>`                         | Last<br>`let x = l.back();`                                                                           |
| `append`              | `fn append(&mut self, &mut LinkedList<T>)`             | Append another list<br>`l1.append(&mut l2);`                                                          |
| `split_off`           | `fn split_off(&mut self, usize) -> LinkedList<T>`      | Split at index<br>`let l2 = l.split_off(2);`                                                          |

### Safe Retrieval Example

```rust
use std::collections::LinkedList;
let mut l = LinkedList::new();
l.push_back(1);
if let Some(x) = l.front() {
    println!("Front: {}", x);
}
```

---

# 9. `BinaryHeap<T>`

A priority queue (max-heap by default).

## All Common Methods

| Method                | Signature & Return Type                                 | Description / Example                                                                                  |
|-----------------------|--------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `new`                 | `fn new() -> BinaryHeap<T>`                            | Empty heap<br>`let h: BinaryHeap<i32> = BinaryHeap::new();`                                           |
| `with_capacity`       | `fn with_capacity(usize) -> BinaryHeap<T>`             | Preallocate<br>`let h = BinaryHeap::with_capacity(10);`                                               |
| `push`                | `fn push(&mut self, T)`                                | Add value<br>`h.push(5);`                                                                             |
| `pop`                 | `fn pop(&mut self) -> Option<T>`                       | Remove max<br>`let x = h.pop();`                                                                      |
| `peek`                | `fn peek(&self) -> Option<&T>`                         | Max value<br>`let x = h.peek();`                                                                      |
| `len`                 | `fn len(&self) -> usize`                               | Length<br>`let n = h.len();`                                                                          |
| `is_empty`            | `fn is_empty(&self) -> bool`                           | Check empty<br>`h.is_empty();`                                                                        |
| `clear`               | `fn clear(&mut self)`                                  | Remove all<br>`h.clear();`                                                                            |
| `iter`                | `fn iter(&self) -> Iter<T>`                            | Iterate<br>`for x in h.iter() {}`                                                                     |
| `clone`               | `fn clone(&self) -> BinaryHeap<T>`                     | Clone<br>`let h2 = h.clone();`                                                                        |
| `extend`              | `fn extend<I: IntoIterator<Item=T>>(&mut self, I)`     | Extend<br>`h.extend([1,2,3]);`                                                                        |
| `drain`               | `fn drain(&mut self) -> Drain<T>`                      | Remove all, iterate<br>`for x in h.drain() {}`                                                        |

### Safe Retrieval Example

```rust
use std::collections::BinaryHeap;
let mut h = BinaryHeap::new();
h.push(10);
if let Some(&x) = h.peek() {
    println!("Max: {}", x);
}
```

---

# Safe Retrieval Patterns

- **`Option<T>`**: Use `match`, `if let`, or `unwrap_or` for safe access.
- **`Result<T, E>`**: Use `match`, `if let`, or `unwrap_or_else` for error handling.

### Example: Safe Access

```rust
let v = vec![1, 2, 3];
let x = v.get(10).unwrap_or(&-1); // &i32, returns -1 if out of bounds

match v.get(1) {
    Some(val) => println!("Value: {}", val),
    None => println!("No value"),
}
```

---

# Summary Table

| Collection      | Key Methods (not exhaustive)                                                                 |
|-----------------|---------------------------------------------------------------------------------------------|
| `Vec<T>`        | `new`, `push`, `pop`, `get`, `get_mut`, `remove`, `insert`, `len`, `is_empty`, `clear`, `contains`, `iter`, `sort`, `reverse`, `resize`, `dedup`, `retain`, `drain`, `capacity`, `shrink_to_fit` |
| `String`        | `new`, `from`, `push_str`, `push`, `len`, `is_empty`, `clear`, `replace`, `contains`, `as_str`, `chars`, `find`, `get`, `insert`, `remove`, `pop`, `split`, `trim`, `to_uppercase`, `to_lowercase`, `starts_with`, `ends_with`, `capacity`, `shrink_to_fit` |
| `HashMap`       | `new`, `insert`, `get`, `get_mut`, `remove`, `contains_key`, `len`, `is_empty`, `clear`, `iter`, `keys`, `values`, `entry`, `drain`, `retain`, `clone`, `extend`, `capacity`, `shrink_to_fit` |
| `HashSet`       | `new`, `insert`, `contains`, `remove`, `len`, `is_empty`, `clear`, `iter`, `drain`, `retain`, `clone`, `extend`, `capacity`, `shrink_to_fit`, `intersection`, `union`, `difference`, `symmetric_difference`, `is_subset`, `is_superset` |
| `VecDeque`      | `new`, `push_back`, `push_front`, `pop_back`, `pop_front`, `get`, `get_mut`, `len`, `is_empty`, `clear`, `iter`, `as_slices`, `front`, `back`, `make_contiguous`, `rotate_left`, `rotate_right` |
| `BTreeMap`      | `new`, `insert`, `get`, `get_mut`, `remove`, `contains_key`, `len`, `is_empty`, `clear`, `iter`, `range`, `first_key_value`, `last_key_value`, `split_off`, `drain_filter` |
| `BTreeSet`      | `new`, `insert`, `contains`, `remove`, `len`, `is_empty`, `clear`, `iter`, `range`, `first`, `last`, `split_off`, `is_subset`, `is_superset`, `intersection`, `union`, `difference`, `symmetric_difference` |
| `LinkedList`    | `new`, `push_back`, `push_front`, `pop_back`, `pop_front`, `len`, `is_empty`, `clear`, `iter`, `front`, `back`, `append`, `split_off` |
| `BinaryHeap`    | `new`, `push`, `pop`, `peek`, `len`, `is_empty`, `clear`, `iter`, `clone`, `extend`, `drain` |

---

**For more, see the [Rust std::collections documentation](https://doc.rust-lang.org/std/collections/).**  
If you want the same for C++ STL, let me know!
