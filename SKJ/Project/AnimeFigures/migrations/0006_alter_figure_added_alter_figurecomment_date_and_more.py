# Generated by Django 5.0.6 on 2024-05-15 16:29

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('AnimeFigures', '0005_alter_figurecomment_date_alter_profilecomment_date'),
    ]

    operations = [
        migrations.AlterField(
            model_name='figure',
            name='added',
            field=models.DateTimeField(auto_now_add=True),
        ),
        migrations.AlterField(
            model_name='figurecomment',
            name='date',
            field=models.DateTimeField(auto_now_add=True),
        ),
        migrations.AlterField(
            model_name='profilecomment',
            name='date',
            field=models.DateTimeField(auto_now_add=True),
        ),
        migrations.AlterField(
            model_name='user',
            name='added',
            field=models.DateTimeField(auto_now_add=True),
        ),
    ]
