import os
from datetime import datetime
from flask import Flask, render_template, session, redirect, url_for, flash
from flask_bootstrap import Bootstrap
from flask_wtf import Form
from wtforms import IntegerField, StringField, SubmitField
from wtforms.validators import Required
from flask_debugtoolbar import DebugToolbarExtension

app = Flask(__name__)
bootstrap = Bootstrap(app)

app.config.from_object('config')

toolbar = DebugToolbarExtension(app)

#首页
@app.route('/')
def index():
	return render_template('index.html')

#链接解析
@app.route('/<name>', methods=['GET', 'POST'])
def showPage(name=None):

	if name=='show_bs':
		doc = ''
		doc_json = []
		device_num = None
		global_status = None
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

			for i in range(0, device_num):
				doc = 'dev/memdev/%s' % i
				for line in open('/dev/memdev%s' % i):
					doc += line.lstrip().rstrip() + '\n'
				doc_json += [doc]
			#os.system('sh exec/test.sh %s %s >> tmp' % (device_num, global_status))
		return render_template('show_bs.html', form=form, device_num=session.get('device_num'), global_status=session.get('global_status'), doc_json=doc_json)

	if name==None:
		redirect(url_for('404.html'))
	return render_template('%s.html' % name)

#404页面
@app.errorhandler(404)
def page_not_found(e):
	return render_template('404.html'), 404

@app.route('/submit')
def submit():
	return '<h1>%s</h1>' % device_num

#表单类
class NameForm(Form):
	device_num = IntegerField(label='输入要创建设备个数：', validators=[Required()])
	global_status = StringField(label='输入状态位：', validators=[Required()])
	submit = SubmitField(label='演示项目')

if __name__ == '__main__':
    app.run(host='0.0.0.0')
