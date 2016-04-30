import os
from datetime import datetime
from flask import Flask, Markup, render_template, session, redirect, url_for, flash
from flask_bootstrap import Bootstrap
from flask_wtf import Form
from wtforms import IntegerField, StringField, SubmitField
from wtforms.validators import Required
from flask_debugtoolbar import DebugToolbarExtension
from flask.ext.misaka import Misaka

app = Flask(__name__)
bootstrap = Bootstrap(app)
Misaka(app)

app.config.from_object('config')

toolbar = DebugToolbarExtension(app)

#首页
@app.route('/')
def index():
	return render_template('index.html')

#链接解析
@app.route('/show_bs', methods=['GET', 'POST'])
def show_bs(name=None):

	doc = ''
	doc_json = []
	
	device_num = None
	global_status = None
	cpu_time = None

	cpuForm = CpuForm()
	if cpuForm.validate_on_submit():
		session['cpu_time'] = cpuForm.cpu_time.data

	form = NameForm()
	if form.validate_on_submit():
		session['device_num'] = form.device_num.data
		session['global_status'] = form.global_status.data

		device_num = session.get('device_num')
		if device_num is not None and device_num not in range(0, 11):
			flash('请输入0-10之间的整数!')
			return redirect(url_for('showPage', name='show_bs'))
		
		global_status = session.get('global_status')
		if global_status is not None and not len(global_status.split(',')) == device_num:
		 	flash('请输入%s个状态位并以逗号分开!' % device_num)
		 	return redirect(url_for('showPage', name='show_bs'))

		os.system('exec/main.sh %s %s' % (device_num, global_status))

		for i in range(0, device_num):
			newline = []
			for line in open('/dev/memdev%s' % i):
				results = line.split('：')
				if len(results) == 2:
					newline += results[1].lstrip().rstrip()
			doc_json += [newline]
		
	return render_template('show_bs.html', form=form, device_num=session.get('device_num'), global_status=session.get("global_status"), cpu_time=session.get('cpu_time'), doc_json=doc_json)

@app.route('/bs_schedule')
def bs_schedule():
	content = ""
	with open("README.md", "r") as readme:
		content = readme.read()
	return render_template('bs_schedule.html', content=content)

#404页面
@app.errorhandler(404)
def page_not_found(e):
	return render_template('404.html'), 404

#表单类
class NameForm(Form):
	device_num = IntegerField(label='输入要创建设备个数：', validators=[Required()])
	global_status = StringField(label='输入状态位：', validators=[Required()])
	submit = SubmitField(label='演示项目')

class CpuForm(Form):
	cpu_time = IntegerField(label="输入想要限制的CPU比例：", validators=[Required()])
	submit = SubmitField(label="限制CPU")

if __name__ == '__main__':
    app.run(host='0.0.0.0')
