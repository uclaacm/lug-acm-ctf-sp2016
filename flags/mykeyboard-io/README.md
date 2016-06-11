## MyKeyboard.io

__Description__

```
MyKeyboard.io is the hot new start-up in town, inspired by a series of popular
posts in a certain computer science Facebook group. Being in closed beta, it
will only allow users with special invite codes to sign up. But like most
hackathon-project-turned-start-ups, MyKeyboard.io doesn't seem to pay much
attention to the security of their flashy website.

http://ctf2.linux.ucla.edu:9001/
```

__Hint__

```
What happens when you try to successfully register again? How is the app doing
this?
```

__Flag__

```
okvulftumhctgjot
```

__Points__

```
80
```

## Solution

The source code for the website is in the `mykeyboard` folder. Credits to
[Start Bootstrap](https://github.com/BlackrockDigital/startbootstrap-agency)
for the design template.

This was one of the two 80-point problems (albeit I was angrily told it should
be worth less ;). The fundamental technique here is the classic **SQL injection**.

Looking at the website, there are many fancy JavaScript functions and CSS
rules, but all of that is just camouflage. The only important part of the
website (besides the memes) is the invite form.

![Sign Up](docs/signup.png)

Whenever there is some kind of form on a website that requires checking
credentials, that should suggest a possibility of a SQL injection
vulnerability. Your objective would be to get past that check.

Entering the wrong code will trigger the following error:

```
Sorry, the following error occurred: Invalid invite code
```

How might an application be making this check? The typical assumption of SQL
injectors is that the app is making a query of this form:

```
SELECT something FROM sometable WHERE code = '{INPUT}'
```

That `{INPUT}` is where the code you enter goes. If the query returns
something, then the check passes. If it doesn't, then it fails. This is really
easy to defeat: just use `' OR 1=1 --` as your code. When the app substitutes
it into the query string, the query becomes:

```
SELECT something FROM sometable WHERE code = '' OR 1=1 --'
```

The query thus always succeeds in finding something, and the check passes. This
isn't the end of the challenge, however. After entering this code, the app
replies with:

```
Your signup has been received and will be processed!
```

Now what? Where's the flag? The challenge hint suggests you try submitting the
request again:

```
Sorry, the following error occurred: The email "my@email.com" is already used
by myname
```

It appears that the app remembers who submitted a valid request and checks to
make sure the next person doesn't accidentally use the same email. How is it
doing this? Well, with another SQL query of course!  Probably of the form:

```
SELECT name, email FROM sometable WHERE email = '{INPUT}'
```

This is likely the hardest part of the challenge, because it's not trivial to
figure out how to proceed. Here, the app isn't doing any gatekeeping checks;
it's merely trying to be helpful and report information to the user. But this
very act of helpfulness can be a vulnerability. Consider an injection on the email field of `' OR email > 'my@email.com' --`. The query becomes:

```
SELECT name, email FROM sometable WHERE email = '' OR email > 'my@email.com' --'
```

This query will thus find the _next user_ in the table, and tell you their name
and email, without you knowing the email to input beforehand! You can use other
comparisons to go forwards or backwards. This is an information-leaking type of
SQLi, which can be just as common as check-bypassing SQLi.

Unfortunately, the webapp helpfully tells you that `' OR email > 'my@email.com'
--` is not a valid email address, and thus disables the submit button. However,
this is merely a frontend restriction; savvy web developers are probably able
to manually invoke the submission via some dev console magic. I prefer to stick
to __curl__:

```
curl -X POST ctf2.linux.ucla.edu:9001/signup    \
    -d name=myname                              \
    -d email="' OR email > 'my@email.com' --"   \
    -d code="' OR 1=1 --"
```

After executing this, the app relies with:

```
{"error": "The email \"okvulftumhctgjot\" is already used by flag", 
 "success": false}
```

And your job is done.

